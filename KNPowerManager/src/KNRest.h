#ifndef _KNRest_H
#define _KNRest_H

#include "Declarations.h"

struct FuncParam {
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

typedef void (*funcCallback)(Vector<FuncParam*>);

/// <summary>
/// RESTful API Manager.
/// </summary>
class KNRest
{
private:
	enum RequestType {GET, POST, PUT, DELETE, UNKNOWN};
	enum CommandType {FUNC, VAR, INVALID};

	struct Variable {};
	template<typename T>
	struct TypedVariable : Variable {
		T* var;

		TypedVariable(T* v) { var = v; }
	};

	EthernetServer*	_server = NULL;
	
	/// <summary>
	/// Functions array
	/// </summary>

	uint8_t	_funcIndex = 0;
	funcCallback _funcCallback[MAX_FUNC];
	const char* _funcNames[MAX_FUNC];
	
	/// <summary>
	/// Variables array
	/// </summary>

	uint8_t _varIndex = 0;
	Variable* _varStruct[MAX_VAR];
	const char* _varNames[MAX_VAR];
		
	/// <summary>
	/// Gets the index.
	/// </summary>
	/// <param name="name">The name.</param>
	/// <param name="array">The array.</param>
	/// <returns></returns>
	uint8_t GetIndex(String name, const char** stringArray, int size);

	/// <summary>
	/// Gets the index of the function.
	/// </summary>
	/// <param name="name">The name.</param>
	/// <returns></returns>
	uint8_t GetFuncIndex(String name);
	
	/// <summary>
	/// Gets the index of the variable.
	/// </summary>
	/// <param name="name">The name.</param>
	/// <returns></returns>
	uint8_t GetVarIndex(String name);
	
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
	/// Adds a variable.
	/// </summary>
	/// <param name="name">The name.</param>
	/// <param name="var">The variable.</param>
	template<typename T>
	void AddVariable(const char* name, T* var);
	
	/// <summary>
	/// Adds a function.
	/// </summary>
	/// <param name="name">The name.</param>
	/// <param name="callback">The callback.</param>
	void AddFunc(const char* name, funcCallback callback);

	/// <summary>
	/// Process (in running loop).
	/// </summary>
	void Process();
};

#endif
