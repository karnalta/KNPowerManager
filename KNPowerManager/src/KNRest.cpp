#include "KNRest.h"

uint8_t KNRest::GetIndex(String name, const char** stringArray, int size)
{
	for (int i = 0; i < size; i++)
	{
		String toCompare = String(stringArray[i]);
		toCompare.toUpperCase();
		name.toUpperCase();

		if (toCompare.startsWith(name))
			return i;
	}

	return size;
}

uint8_t KNRest::GetFuncIndex(String name)
{
	return GetIndex(name, _funcNames, MAX_FUNC);
}

uint8_t KNRest::GetVarIndex(String name)
{
	return GetIndex(name, _varNames, MAX_VAR);
}

void KNRest::ReplyOk(EthernetClient client)
{
	client.println("HTTP/1.1 200 OK");
}

void KNRest::ReplyError(EthernetClient client)
{
	client.println("HTTP/1.1 400 BAD REQUEST");
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

			// Parse func name
			uint8_t spaceIndex = request.indexOf(' ');
			request = request.substring(6, spaceIndex);

			// Retrieve the function
			uint8_t index = GetFuncIndex(request.c_str());	

			if (index < MAX_FUNC)
			{
				KNLog::LogEvent(&(knrest_table[3]), false);
				KNLog::LogEvent(request, true, false);

				_funcCallback[index]();

				ReplyOk(client);
				handled = true;
			}
			else
				ReplyError(client);
		}
		else if (request.startsWith("/var/"))
		{
			commandType = CommandType::VAR;

			// Parse variable name
			uint8_t spaceIndex = request.indexOf(' ');
			request = request.substring(5, spaceIndex);

			// Retrieve the variable
			uint8_t index = GetVarIndex(request.c_str());

			if (index < MAX_VAR)
			{
				KNLog::LogEvent(&(knrest_table[4]), false);
				KNLog::LogEvent(request, true, false);
				
				ReplyOk(client);
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
		_funcNames[i] = "";

	for (int i = 0; i < MAX_VAR; i++)
		_varNames[i] = "";
}

void KNRest::AddFunc(const char* name, funcCallback callback)
{
	if (_funcIndex >= (MAX_FUNC - 1))
		return;

	_funcNames[_funcIndex] = name;
	_funcCallback[_funcIndex] = callback;

	_funcIndex++;
}

template<typename T>
void KNRest::AddVariable(const char* name, T* var)
{
	if (_varIndex >= (MAX_VAR - 1))
		return;

	_varNames[_varIndex] = name;
	_varStruct[_varIndex] = new TypedVariable<T>(var);

	_varIndex++;
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
