

typedef ezUInt32 ezPluginModule;

void ezPlugin::GetPluginPaths(const char* szPluginName, ezStringBuilder& sOldPath, ezStringBuilder& sNewPath)
{
}

ezResult UnloadPluginModule(ezPluginModule& Module, const char* szPluginFile)
{
  return EZ_SUCCESS;
}

ezResult LoadPluginModule(const char* szFileToLoad, ezPluginModule& Module, const char* szPluginFile)
{
  return EZ_SUCCESS;
}

