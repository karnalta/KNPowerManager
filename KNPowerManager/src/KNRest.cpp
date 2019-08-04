#include "KNRest.h"

uint8_t KNRest::GetFuncIndex(String name)
{
	for (int i = 0; i < MAX_FUNC; i++)
	{
		String toCompare = String(_functions[i].Name);
		toCompare.toUpperCase();
		name.toUpperCase();

		if (toCompare.startsWith(name))
			return i;
	}

	return MAX_FUNC;
}

void KNRest::AddFunc(const char* name, void* callback, CallbackType type)
{
	if (_currentFuncIndex >= (MAX_FUNC - 1))
		return;

	_functions[_currentFuncIndex].Name = name;
	_functions[_currentFuncIndex].Pointer = callback;
	_functions[_currentFuncIndex].Type = type;

	_currentFuncIndex++;
}

void KNRest::ReplyOk(EthernetClient client)
{
	client.println("HTTP/1.1 200 OK");
}

void KNRest::ReplyError(EthernetClient client)
{
	client.println("HTTP/1.1 400 BAD REQUEST");
}

void KNRest::ReplyJSON(EthernetClient client, Vector<FuncParam*>* result)
{
	if (result == NULL)
		return;

	client.println("{");

	for (int i = 0; i < result->Size(); i++)
	{
		client.print("\""); client.print((*result)[i]->Name); client.print("\"");
		client.print(": ");
		client.print("\""); client.print((*result)[i]->Value); client.print("\"");

		if (i == (result->Size() - 1))
			client.println("");
		else
			client.println(",");
	}

	client.println("}");
}

void KNRest::DecodeParams(String args, Vector<FuncParam*>* params)
{
	if (args == "")
		return;

	// Parse
	bool parsing = true;
	while (parsing)
	{
		int index = args.indexOf('&');
		String param = args;

		// Check if there is another param after this one
		if (index > 0)
		{
			param = args.substring(0, index);
			args = args.substring(index + 1);
		}
		else
			parsing = false;

		// Decode param
		int equalIndex = param.indexOf('=');
		if (equalIndex > 0)
		{
			String key = param.substring(0, equalIndex);
			String value = param.substring(equalIndex + 1);

			params->PushBack(new FuncParam(key, value));
		}
	}
}

void KNRest::HandleRequest(String request, EthernetClient client)
{
	RequestType requestType = RequestType::UNKNOWN;
	bool handled = false;

	// Get request type
	if (request.startsWith("GET "))
	{
		requestType = RequestType::GET;
		request = request.substring(4);
	}
	else
	{
		ReplyError(client);
		return;
	}

	// Handle GET
	if (requestType == RequestType::GET)
	{
		CommandType commandType = INVALID;

		if (request.startsWith("/func/"))
		{
			commandType = CommandType::FUNC;

			// Parse command
			int spaceIndex = request.indexOf(' ');
			request = request.substring(6, spaceIndex);

			// Parse func name and args
			int slashIndex = request.indexOf('?');
			String funcName = request;
			if (slashIndex > 0)
				funcName = request.substring(0, slashIndex);
			String args = request.substring(slashIndex + 1);

			// Retrieve the function
			int index = GetFuncIndex(funcName.c_str());

			// Decode params
			Vector<FuncParam*> params; 
			DecodeParams(args, &params);

			// Callback
			if (index < MAX_FUNC)
			{
				KNLog::LogEvent(&(knrest_table[3]), false);
				KNLog::LogEvent(funcName, true, false);

				if (_functions[index].Type == CallbackType::VOID)
				{
					((voidCallback)(_functions[index].Pointer))(params);
					ReplyOk(client);
				}
				else if (_functions[index].Type == CallbackType::JSON)
				{
					//const Vector<FuncParam*> result = ((jsonCallback)(_functions[index].Pointer))(params);
					ReplyJSON(client, &(((jsonCallback)(_functions[index].Pointer))(params)));
				}

				handled = true;
			}
			else
				ReplyError(client);
		}

		// Unknow command
		if (commandType == INVALID)
		{
			ReplyError(client);
			return;
		}
	}

	if (!handled)
		KNLog::LogEvent(&(knrest_table[5]));
}

KNRest::KNRest(byte* ip, byte* mac, byte* gateway, byte* subnet)
{
	// Init TCP/IP
	Ethernet.begin(mac, ip, gateway, gateway, subnet);

	// Init WebServer
	_server = new EthernetServer(80);
	_server->begin();

	// Init array
	for (int i = 0; i < MAX_FUNC; i++)
	{
		_functions[i] = CallbackFunction();
	}
}

void KNRest::AddFunc(const char* name, voidCallback callback)
{
	AddFunc(name, callback, CallbackType::VOID);
}

void KNRest::AddFunc(const char* name, jsonCallback callback)
{
	AddFunc(name, callback, CallbackType::JSON);
}

void KNRest::Process()
{
	// Listen for incoming clients
	EthernetClient client = _server->available();
	if (client)
	{
		// Log
		KNLog::LogEvent(&(knrest_table[0]));

		int currentReqBufferIndex = 0;
		String request = "";

		while (client.available())
		{
			// Read
			char c = client.read();

			// Check for HTTP request end
			if (c == '\n')
			{
				KNLog::LogEvent(&(knrest_table[2]), false, true);
				KNLog::LogEvent(request, true, false);

				// Handle
				HandleRequest(request, client);

				// Reset
				currentReqBufferIndex = 0;
				request = "";
				break;
			}

			// Storing max x char of the HTTP request first line
			if (currentReqBufferIndex < (HTTP_BUFFER_SIZE - 1))
			{
				request += c;
				currentReqBufferIndex++;
			}
		}

		// Give the web browser time to receive the data
		delay(1);

		// Close the connection
		client.stop();
	}
}
