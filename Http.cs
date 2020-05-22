using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks; 
using Newtonsoft.Json.Linq;
using System.Collections;

namespace WindowsFormsApplication1
{
    class Http
    {

        static public ArrayList Get(string text)
        {
            WebClient client = new WebClient();
            client.Headers.Add("user-agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; .NET CLR 1.0.3705;)");

            //Stream data = client.OpenRead("http://139.198.127.54:18240");
            //StreamReader reader = new StreamReader(data);
            //string s = reader.ReadToEnd();
            //Console.WriteLine(s);
            //data.Close();
            //reader.Close();

            ArrayList rs = new ArrayList();
            JArray arr = JArray.Parse("[['foo'],['bar','123']]");
            foreach (Object item in arr.Children())
            {
                Console.WriteLine(item.GetType());
                if (item.GetType() == typeof(JArray))
                {
                    JArray items = (JArray)item;
                    rs.Add(items.Select(jv => (string)jv).ToArray());
                }
            }
            return rs; 
        }
    }
}
