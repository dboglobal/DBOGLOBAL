using System;
using System.Collections.Generic;
using System.Text;

namespace ICSService
{
    class ICSLauncherMap
    {
        static private Dictionary<string, string> mapLauncher = new Dictionary<string,string>();

        static public void SetLauncher(string key, string path)
        {
            mapLauncher.Add(key, path);
        }

        static public string GetLauncher(string key)
        {
            string path;
            if(!mapLauncher.TryGetValue(key, out path))
            {
                return null;
            }

            return path;
        }
    }
}
