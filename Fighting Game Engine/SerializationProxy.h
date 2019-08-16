#pragma once
#include "JSON.h"
#include "LuaUtil.h"
#include "IReflectable.h"
#include "ObjectFactory.h"
#include "TemplateUtils.h"

class BaseReflectionField;

class BaseSerializationProxy
{
public:
	template<typename VarType>
	bool Get(const std::string& name, VarType& out_var) const;
	template<typename VarType>
	void Set(const std::string& name, const VarType& var);

	template<typename VarType>
	void GetObject(const std::string& name, ve::unique_object_ptr<VarType>& out_var, BaseObject* outer) const;

	template<typename VarType>
	bool GetArray(const std::string& name, std::vector<VarType>& out_vec)const;
	template<typename VarType>
	void SetArray(const std::string& name, const std::vector<VarType>& vec);

	template<typename VarType>
	void FillObjectArray(const std::string& name, std::vector<VarType>& out_vec, BaseObject* outer) const;

	template<typename KeyType, typename ValueType>
	bool GetMap(const std::string& name, std::unordered_map<KeyType, ValueType>& out_map)const;
	template<typename KeyType, typename ValueType>
	void SetMap(const std::string& name, const std::unordered_map<KeyType, ValueType>& vec);

	virtual std::unique_ptr<BaseSerializationProxy> GetSubProxy(const std::string& name) const { return std::make_unique<BaseSerializationProxy>(); }

	BaseSerializationProxy() = default;
	virtual ~BaseSerializationProxy() = default;
};

class JsonSerializationProxy : public BaseSerializationProxy
{
protected:
	json& _json;

public:
	const json& data() const { return _json; }

	template<typename VarType>
	bool Get(const std::string& name, VarType& out_var)const;
	template<typename VarType>
	void Set(const std::string& name, const VarType& var);

	template<typename VarType>
	void GetObject(const std::string& name, ve::unique_object_ptr<VarType>& out_var, BaseObject* outer) const;

	template<typename VarType>
	bool GetArray(const std::string& name, std::vector<VarType>& out_vec)const;
	template<typename VarType>
	void SetArray(const std::string& name, const std::vector<VarType>& vec);

	template<typename VarType>
	void FillObjectArray(const std::string& name, std::vector<VarType>& out_vec, BaseObject* outer) const;

	template<typename KeyType, typename ValueType>
	bool GetMap(const std::string& name, std::unordered_map<KeyType, ValueType>& out_map)const;
	template<typename KeyType, typename ValueType>
	void SetMap(const std::string& name, const std::unordered_map<KeyType, ValueType>& vec);

	std::unique_ptr<BaseSerializationProxy> GetSubProxy(const std::string& name) const override;

	JsonSerializationProxy(json& j)
		:_json(j) {}
};

class LuaSerializationProxy : public BaseSerializationProxy
{
protected:
	ve::LuaState& _state;
	std::string _objectName;

public:
	lua::Selector data() const { return _state[_objectName.c_str()]; }

	template<typename VarType>
	bool Get(const std::string& name, VarType& out_var)const;
	template<typename VarType>
	void Set(const std::string& name, const VarType& var);

	template<typename VarType>
	void GetObject(const std::string& name, ve::unique_object_ptr<VarType>& out_var, BaseObject* outer) const;

	template<typename VarType>
	bool GetArray(const std::string& name, std::vector<VarType>& out_vec)const;
	template<typename VarType>
	void SetArray(const std::string& name, const std::vector<VarType>& vec);

	template<typename VarType>
	void FillObjectArray(const std::string& name, std::vector<VarType>& out_vec, BaseObject* outer)const;

	template<typename KeyType, typename ValueType>
	bool GetMap(const std::string& name, std::unordered_map<KeyType, ValueType>& out_map)const;
	template<typename KeyType, typename ValueType>
	void SetMap(const std::string& name, const std::unordered_map<KeyType, ValueType>& vec);

	std::unique_ptr<BaseSerializationProxy> GetSubProxy(const std::string& name) const override;

	LuaSerializationProxy(ve::LuaState& state, std::string objectName)
		:_state(state)
		, _objectName(std::move(objectName)) {}
};

#include "BaseSerializationProxy_Impl.h"
#include "LuaSerializationProxy_Impl.h"
#include "JsonSerializationProxy_Impl.h"