
template<class K, class V>
Manager<K, V>::Manager(const std::string &logTag, IManagedObjectBuilder<K, V> *builder) :
	logTag(logTag),
	builder(builder)
{
	// Compile-time sanity check
	static_assert(std::is_base_of<ManagedObject, V>::value, "Creating a Manager of Values not derived from ManagedObject");

	LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
		logTag, "Initialized successfully.");
}

template<class K, class V>
Manager<K, V>::~Manager()
{
	clear();

	delete builder;

	LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
		logTag, "Destroyed successfully.");
}

template<class K, class V>
Handle<V> Manager<K, V>::get(const K &name)
{
	LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
		logTag, "Getting object with name: " + toString(name));

	auto it = objects.find(name);
	V* object;
	ControlBlock<V>* controlBlock;

	if (it == objects.end())
	{
		object = builder->build(name);
		object->setName(toString(name));
		if (object == nullptr)
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Error building Object " + toString(name));
			return Handle<V>();
		}

		objects[name] = object;

		controlBlock = new ControlBlock<V>(object);
		controlBlocks[name] = controlBlock;

		LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
			logTag, "Created object with name: " + toString(name));
	}
	else
	{
		object = it->second;
		controlBlock = controlBlocks[name];
		LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
			logTag, "Getting already existent object with name: " + toString(name));
	}

	if (load(object))
		return Handle<V>(*controlBlock);
	else
	{
		unload(object);
		return Handle<V>();
	}
}

template<class K, class V>
bool Manager<K, V>::contains(const K &name) const
{
	auto it = objects.find(name);
	return it != objects.end();
}

template<class K, class V>
void Manager<K, V>::clear()
{
	for (auto &pair : objects)
	{
		unload(pair.second);
		delete pair.second;
	}

	for (auto &pair : controlBlocks)
	{
		delete pair.second;
	}

	objects.clear();
	controlBlocks.clear();

	LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
		logTag, "Cleared all objects successfully.");
}

template<class K, class V>
void Manager<K, V>::clearUnused()
{
	int count = 0;
	for (auto it = objects.begin(); it != objects.end(); )
	{
		auto &objectPair = *it;
		ControlBlock<V> *controlBlock = controlBlocks[objectPair.first];

		if (!controlBlock->hasReferences())
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
				logTag, "Clearing unused object: " + objectPair.second->getName() + ".");

			unload(objectPair.second);
			delete objectPair.second;
			delete controlBlock;

			it = objects.erase(it);
			controlBlocks.erase(objectPair.first);
			++count;
		}
		else
			++it;
	}

	LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
		logTag, "Cleared " + std::to_string(count) + " unused objects.");
}

template<class K, class V>
bool Manager<K, V>::load(V* object) const
{
	if (object->load())
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
			logTag, "Loaded object: " + object->getName());
		return true;
	}
	else
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
			logTag, "Failed to load object: " + object->getName());
		return false;
	}
}

template<class K, class V>
void Manager<K, V>::unload(V* object) const
{
	object->unload();
	LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
		logTag, "Unloaded object: " + object->getName());
}