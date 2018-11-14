
#include"volte_config.h"

void test_config_0()
{
    int id = 0x1210;
    string mem_add = "";
    string proc_add = "";
    char filename[64];
    char sed_script[64];
    int k = sizeof(g_proc_child_name) / sizeof(g_proc_child_name[0]);

    vector < string > file_vector[4 * k];
    vector < string > cp_file_vecotr;
    cp_file_vecotr.clear();

    //内存
    cout << "#########内存定义########################" << endl;
    for (int fn = 1; fn <= 4; fn++)
    {
        for (int pn = 0; pn < k; pn++)
        {
            string strCmd = "sed -i '/memPdpSesn...size/a \\\\n";
            string strCmdport = "sed -i '/pdpImport/a ";
            string strCmdimport = "queryExport=";
            string strCmdexport = "\\nqueryImport=";
            string strCmdList = "";
            {
#if 0

                //复制文件
                memset(filename, 0, sizeof(filename));
                sprintf(filename, "cp sesn%dE.conf sesn%d%s.conf", fn, fn, g_proc_child_name[pn]);
                cp_file_vecotr.push_back(filename);
                memset(filename, 0, sizeof(filename));
                sprintf(filename, "cp docTcpip%dE.conf docTcpip%d%s.conf", fn, fn, g_proc_child_name[pn]);
                cp_file_vecotr.push_back(filename);
                memset(filename, 0, sizeof(filename));
                sprintf(filename, "cp csvWCDR%dE.conf csvWCDR%d%s.conf", fn, fn, g_proc_child_name[pn]);
                cp_file_vecotr.push_back(filename);

                //替换主机号
                memset(filename, 0, sizeof(filename));
                sprintf(filename, "sed -i '/RCI=/c RCI=%d%d' sesn%d%s.conf", fn, pn + 6, fn, g_proc_child_name[pn]);
                cp_file_vecotr.push_back(filename);
                memset(filename, 0, sizeof(filename));
                sprintf(filename, "sed -i '/csv_prefix=/c csv_prefix=VoLTE_54_%d%d' csvWCDR%d%s.conf", fn, pn + 6, fn, g_proc_child_name[pn]);
                cp_file_vecotr.push_back(filename);
#endif
            }
            int vn = (fn - 1) *k + pn;

            file_vector[vn].clear();
            for (int mn = 0; mn < sizeof(g_mem_name) / sizeof(g_mem_name[0]); mn++)
            {
                char proc0[32], proc[32], size[32];
                char proc2[64], size2[64];

                memset(proc0, 0, sizeof(proc0));
                memset(proc, 0, sizeof(proc));
                memset(size, 0, sizeof(size));
                memset(proc2, 0, sizeof(proc2));
                memset(size2, 0, sizeof(size2));
                char * p1 = strstr(g_mem_name[mn], "_");

                strncpy(proc, g_mem_name[mn], p1 - g_mem_name[mn]);
                strcpy(size, p1 + 1);
                sprintf(proc0, "%s%d%s", proc, fn, "E");
                sprintf(proc, "%s%d%s", proc, fn, g_proc_child_name[pn]);
                sprintf(proc2, "%s.id=0x%x", proc, id++);
                sprintf(size2, "%s.size=%s", proc, size);
                cout << proc2 << endl;
                cout << size2 << endl;
                strCmd += proc2;
                strCmd += "\\n";
                strCmd += size2;
                strCmd += "\\n";
                if (strstr(proc, "memCmdQuery") != NULL)
                {
                    strCmdimport += string(proc) + ",";
                    if (strstr(proc, "memCmdQueryXdr"))
                    {
                        strCmdList += "\\nlistFlag." + string(proc) + "=3";
                    }
                    else 
                        strCmdList += "\\nlistFlag." + string(proc) + "=1-9";
                }
                else if (strstr(proc, "memCmdResult") != NULL)
                {
                    strCmdexport += string(proc) + ",";
                }
                cout << endl;
                if (NULL == strstr(proc, "memWS"))
                {
                    mem_add += string(proc) + ",";
                }
                memset(sed_script, 0, sizeof(sed_script));
                sprintf(sed_script, "sed -i '/%s.id=/c %s' *%d%s.conf", proc0, proc2, fn, g_proc_child_name[pn]);
                file_vector[vn].push_back(sed_script);
                memset(sed_script, 0, sizeof(sed_script));
                sprintf(sed_script, "sed -i '/%s.size=/c %s' *%d%s.conf", proc0, size2, fn, g_proc_child_name[pn]);
                file_vector[vn].push_back(sed_script);
                memset(sed_script, 0, sizeof(sed_script));
                sprintf(sed_script, "sed -i 's/%s/%s/g' *%d%s.conf", proc0, proc, fn, g_proc_child_name[pn]);
                file_vector[vn].push_back(sed_script);
            }
            char sCmd[32] =
            {
                0
            };
            sprintf(sCmd, "' sesn%d%s.conf", fn, g_proc_child_name[pn]);
            strCmd += sCmd;
            strCmdimport = strCmdimport.substr(0, strCmdimport.length() - 1);
            strCmdexport = strCmdexport.substr(0, strCmdexport.length() - 1);
            strCmdport += strCmdimport;
            strCmdport += strCmdexport;
            strCmdport += strCmdList;
            strCmdport += sCmd;

            //cout<<strCmd<<endl;
            //cout<<strCmdport<<endl;
        }
    }
    //进程
    cout << "#########进程定义########################" << endl;
    for (int fn = 1; fn <= 4; fn++)
    {
        for (int pn = 0; pn < k; pn++)
        {
            for (int mn = 0; mn < sizeof(g_proc_name) / sizeof(g_proc_name[0]); mn++)
            {
                char * p1 = strstr(g_proc_name[mn], "_");
                char * p2 = strstr(p1 + 1, "_");
                char proc[32], exe[32], sequen[2];

                memset(proc, 0, sizeof(proc));
                memset(exe, 0, sizeof(exe));
                memset(sequen, 0, sizeof(sequen));
                strncpy(proc, g_proc_name[mn], p1 - g_proc_name[mn]);
                strncpy(sequen, p1 + 1, p2 - p1 - 1);
                strcpy(exe, p2 + 1);
                sprintf(proc, "%s%d%s", proc, fn, g_proc_child_name[pn]);

                /*
                cout << proc << ".exe=" << exe << endl;
                cout << proc << ".param=" << proc << endl;
                cout << proc << ".sequen=" << sequen << endl;
                cout << endl;
                */
                if (NULL == strstr(proc, "WSCode"))
                {
                    proc_add += string(proc) + ",";
                }
            }
        }
    }
    cout << "###启动内存和进程##########################" << endl;

    //cout << proc_add << endl;
    cout << mem_add << endl;
    cout << "###文件copy###################" << endl;
    for (int i = 0; i < cp_file_vecotr.size(); i++)
    {
        //cout<<cp_file_vecotr[i]<<endl;
    }
    cout << "###文件处理############################" << endl;
    for (int j = 0; j < 4 * k; j++)
    {
        for (int i = 0; i < file_vector[j].size(); i++)
        {
            //cout<<file_vector[j][i]<<endl;
        }
        //cout << "#################################" << endl;
    }
}

