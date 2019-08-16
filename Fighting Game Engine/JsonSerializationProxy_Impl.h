#pragma once

template <typename VarType>
bool JsonSerializationProxy::Get(const std::string& name, VarType& out_var) const
{
	json j;
	if (!JSON::TryGetMember(_json, name, j))
	{
		return false;
	}

	if constexpr (std::is_base_of_v<IReflectable, ve::remove_object_ptr_t<VarType>>)
	{
		JsonSerializationProxy proxy(j);
		if constexpr (ve::is_object_ptr_v<VarType>)
		{
			if (out_var != nullptr)
			{
				out_var->DeserializeProxy(proxy);
			}
		}
		else
		{
			out_var.DeserializeProxy(proxy);
		}
	}
	else if constexpr (std::is_same_v<VarType, json>)
	{
		out_var = j;
	}
	else
	{
		out_var = JSON::Get<VarType>(j);
	}

	return true;
}

template <typename VarType>
void JsonSerializationProxy::Set(const std::string& name, const VarType& var)
{
	if constexpr (std::is_base_of_v<IReflectable, ve::remove_object_ptr_t<VarType>>)
	{
		JsonSerializationProxy proxy(_json[name]);
		if constexpr (ve::is_object_ptr_v<VarType>)
		{
			if (var != nullptr)
			{
				var->SerializeProxy(proxy);
			}
		}
		else
		{
			var.SerializeProxy(proxy);
		}
	}
	else if constexpr (std::is_same_v<VarType, json>)
	{
		_json[name] = var;
	}
	else
	{
		_json[name] = JSON::ToJson<VarType>(var);
	}
}

template <typename VarType>
bool JsonSerializationProxy::GetArray(const std::string& name, std::vector<VarType>& out_vec) const
{
	json j;
	if (!JSON::TryGetMember(_json, name, j) || !j.is_array())
	{
		return false;
	}

	if constexpr (std::is_base_of_v<IReflectable, ve::remove_object_ptr_t<VarType>>)
	{
		const size_t minSize = out_vec.size() < j.size() ? out_vec.size() : j.size();
		for (int i = 0; i < minSize; ++i)
		{
			JsonSerializationProxy proxy(j[i]);
			if constexpr (ve::is_object_ptr_v<VarType>)
			{
				out_vec[i]->DeserializeProxy(proxy);
			}
			else
			{
				out_vec[i].DeserializeProxy(proxy);
			}
		}
	}
	else
	{
		out_vec.clear();
		out_vec.reserve(j.size());
		for (const json& iter : j)
		{
			if constexpr (std::is_same_v<VarType, json>())
			{
				out_vec.emplace_back(iter);
			}
			else
			{
				out_vec.emplace_back(JSON::Get<VarType>(iter));
			}
		}
	}

	return true;
}

template <typename VarType>
void JsonSerializationProxy::SetArray(const std::string& name, const std::vector<VarType>& vec)
{
	json& j = _json[name];

	for (size_t i = 0; i < vec.size(); ++i)
	{
		const VarType& iter = vec[i];
		if constexpr (std::is_base_of_v<IReflectable, ve::remove_object_ptr_t<VarType>>)
		{
			json sjson;
			JsonSerializationProxy proxy(sjson);
			if constexpr (ve::is_object_ptr_v<VarType>)
			{
				iter->SerializeProxy(proxy);
			}
			else
			{
				iter.SerializeProxy(proxy);
			}
			j.emplace_back(sjson);
		}
		else if constexpr (std::is_same_v<VarType, json>())
		{
			j.emplace_back(iter);
		}
		else
		{
			j.emplace_back(JSON::ToJson<VarType>(iter));
		}
	}
}

template<typename VarType>
void JsonSerializationProxy::GetObject(const std::string& name, ve::unique_object_ptr<VarType>& out_var, BaseObject* outer) const
{
	static_assert(std::is_base_of_v<BaseObject, ve::remove_object_ptr_t<VarType>>);

	json& j = _json[name];
	JsonSerializationProxy proxy{ j };

	if (out_var != nullptr)
	{
		out_var->SerializeProxy(proxy);
	}
	else
	{
		out_var = std::move(ObjectFactory::CreateObjectFromData<ve::remove_object_ptr_t<VarType>>(outer, proxy));
	}

}

template <typename VarType>
void JsonSerializationProxy::FillObjectArray(const std::string& name, std::vector<VarType>& out_vec, BaseObject* outer) const
{
	static_assert(std::is_base_of_v<IReflectable, ve::remove_object_ptr_t<VarType>>);

	json& j = _json[name];

	out_vec.reserve(out_vec.size() + j.size());
	for (auto& iter : j)
	{
		JsonSerializationProxy proxy(iter);

		if constexpr (ve::is_object_ptr_v<VarType>)
		{
			out_vec.emplace_back(ObjectFactory::CreateObjectFromData<ve::remove_object_ptr_t<VarType>>(outer, proxy));
		}
		else
		{
			out_vec.emplace_back(VarType());
			out_vec.back().SerializeProxy(proxy);
		}
	}
}


template <typename KeyType, typename ValueType>
bool JsonSerializationProxy::GetMap(const std::string& name, std::unordered_map<KeyType, ValueType>& out_map) const
{
	json j;
	if (!JSON::TryGetMember(_json, name, j))
	{
		return false;
	}

	out_map->clear();
	for (auto iter = j.begin(); iter != j.end(); ++iter)
	{
		_data->emplace(JSON::Get<KeyType>(iter.key()), JSON::Get<ValueType>(iter.value()));
	}

	return true;
}

template <typename KeyType, typename ValueType>
void JsonSerializationProxy::SetMap(const std::string& name, const std::unordered_map<KeyType, ValueType>& vec)
{
	json& j = _json[name];

	for (auto& iter : vec)
	{
		j.emplace(JSON::ToJson<KeyType>(iter.first), JSON::ToJson<ValueType>(iter.second));
	}
}