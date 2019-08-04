#ifndef _KNRest_H
#define _KNRest_H

#include "Declarations.h"

/// <summary>
/// Container key/value pair.
/// </summary>
struct FuncParam
{
	String Name;
	String Value;

	FuncParam()
	{
		Name = "";
		Value = "";
	}

	FuncParam(String name, String value)
	{
		Name = name;
		Value = value;
	}
};

typedef void (*voidCallback)(Vector<FuncParam*>);
typedef Vector<FuncParam*> (*jsonCallback)(Vector<FuncParam*>);

/// <summary>
/// RESTful API Manager.
/// </summary>
class KNRest
{
private:
	enum RequestType {GET, POST, PUT, DELETE, UNKNOWN};
	enum CommandType {FUNC, INVALID};
	enum CallbackType {VOID, JSON};

	struct CallbackFunction
	{
		String Name = "";
		void* Pointer = nullptr;
		CallbackType Type = CallbackType::VOID;
	};

	uint8_t _currentFuncIndex = 0;
	CallbackFunction _functions[MAX_FUNC];

	EthernetServer* _server = NULL;

	/// <summary>
	/// Gets the index of the function.
	/// </summary>
	/// <param name="name">The name.</param>
	/// <returns></returns>
	uint8_t GetFuncIndex(String name);

	/// <summary>
	/// Adds a function.
	/// </summary>
	/// <param name="name">The name.</param>
	/// <param name="callback">The callback.</param>
	void AddFunc(const char* name, void* callback, CallbackType type);
	
	/// <summary>
	/// Replies an ok.
	/// </summary>
	/// <param name="client">The client.</param>
	void ReplyOk(EthernetClient client);
	
	/// <summary>
	/// Replies an error.
	/// </summary>
	/// <param name="client">The client.</param>
	void ReplyError(EthernetClient client);
	
	/// <summary>
	/// Replies a json result.
	/// </summary>
	/// <param name="client">The client.</param>
	/// <param name="result">The result.</param>
	void ReplyJSON(EthernetClient client, Vector<FuncParam*>* result);
	
	/// <summary>
	/// Decodes the parameters.
	/// </summary>
	/// <param name="args">The arguments.</param>
	/// <returns>An array of FuncParam</returns>
	void DecodeParams(String args, Vector<FuncParam*>* params);

	/// <summary>
	/// Handles the request.
	/// </summary>
	/// <param name="request">The request.</param>
	void HandleRequest(String request, EthernetClient client);

public:	
	/// <summary>
	/// Initializes a new instance of the <see cref="KNRest"/> class.
	/// </summary>
	/// <param name="ip">The ip.</param>
	/// <param name="mac">The mac.</param>
	/// <param name="gateway">The gateway.</param>
	/// <param name="subnet">The subnet.</param>
	KNRest(byte* ip, byte* mac, byte* gateway, byte* subnet);
	
	/// <summary>
	/// Adds a function.
	/// </summary>
	/// <param name="name">The name.</param>
	/// <param name="callback">The callback.</param>
	void AddFunc(const char* name, voidCallback callback);
	   
	/// <summary>
	/// Adds a function.
	/// </summary>
	/// <param name="name">The name.</param>
	/// <param name="callback">The callback.</param>
	void AddFunc(const char* name, jsonCallback callback);

	/// <summary>
	/// Process (in running loop).
	/// </summary>
	void Process();
};

#endif
