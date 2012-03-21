/*
 * 想想软件通常是怎么加载插件的，第一次接触么? 那就仔细看看并记下来。
 *
 * 流程是这样的：
 *	1、先判断系统是否已经加载了，已加载则返回
 *	2、将插件DLL加载到内存中，返回句柄 lib
 *	3、获取DLL入口地址 pFunc，并填写插件入口数据结构PLUGINENTRY
 *	4、将上一步得到的数据填入PluginEntryMap结构中
 *	5、调用 pFunc: pFunc(lib, pluginName)，启动插件服务
 *	6、如果调用失败，就进行内存和“注册表”清理
 *
 * 还有其他细节，不过不是重点。
 */

void OgitorsRoot::LoadPlugin(Ogre::String pluginPath)
{
    PluginEntryMap::iterator it = mPlugins.begin();

    while(it != mPlugins.end())
    {
        PLUGINENTRY ent = it->second;
        if(ent.mFileName == pluginPath)
            return;

        it++;
    }

/*
 * 注意上面几行代码的目的。形式很常见不是---用于匹配的代码。
 * 目的是判断系统是否已经加载了这个插件；
 * 这些加载的插件会记录在PluginEntryMap这个结构中。
 */

    DynLib* lib;
    lib = OGRE_NEW DynLib( pluginPath );

// try 和 catch 不清楚其用途

    try
    {
        lib->load();
    }
    catch(...)
    {
        OGRE_DELETE lib;
        return;
    }

    // Call startup function
    DLL_START_PLUGIN pFunc = (DLL_START_PLUGIN)lib->getSymbol("dllStartPlugin");

    PLUGINENTRY entry;
    entry.mName = "";
    entry.mFileName = pluginPath;
    entry.mLibrary = lib;
    entry.mFeatures = 0;
    entry.mEditorObjects.clear();
    entry.mSerializers.clear();
    mPlugins.insert(PluginEntryMap::value_type((void*)lib, entry));

    it = mPlugins.find(lib);

    Ogre::String pluginName = "";

    if(pFunc && pFunc(lib, pluginName))
    {
        it->second.mName = pluginName;
    }
    else
    {
        mPlugins.erase(it);
        lib->unload();
        OGRE_DELETE lib;
    }
}