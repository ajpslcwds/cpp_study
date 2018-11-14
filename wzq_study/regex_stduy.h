#ifndef REGEX_STUDY_H
#define REGEX_STUDY_H

#include "common.h"
#include <regex>


void test_regex_match()
{
    
    // 简单正则表达式匹配
    string fnames[] = {"foo.txt", "bar.txt", "baz.dat", "zoidberg"};
    regex txt_regex(".*txt"); 

    string fname="";
    for (int i =0 ;i<4;i++){ fname =fnames[i];
        cout << fname << ": " << regex_match(fname, txt_regex) << '\n';
    }


    // 提取子匹配
    regex base_regex("(.*)txt");
    smatch base_match;
 
    for (int i =0 ;i<4;i++){ fname =fnames[i];
        if (regex_match(fname, base_match, base_regex)) {
            // 首个 sub_match 是整个字符串；下个
            // sub_match 是首个有括号表达式。
            if (base_match.size() == 2) {
                ssub_match base_sub_match = base_match[1];
                string base = base_sub_match.str();
                cout << fname << " has a base of " << base << '\n';
            }
        }
    }
  /*
    // 提取几个子匹配
    regex pieces_regex("([a-z]+)\\.([a-z]+)");
    smatch pieces_match;
 
    for (int i =0 ;i<4;i++){ fname =fnames[i];
        if (regex_match(fname, pieces_match, pieces_regex)) {
            cout << fname << '\n';
            for (size_t i = 0; i < pieces_match.size(); ++i) {
                ssub_match sub_match = pieces_match[i];
                string piece = sub_match.str();
                cout << "  submatch " << i << ": " << piece << '\n';
            }   
        }   
    }  
*/
    return ;
}

void test_regex_search()
{

    // 重复搜索（参阅 regex_iterator ）
    string log(R"(
        Speed:	366
        Mass:	35
        Speed:	378
        Mass:	32
        Speed:	400
	    Mass:	30)");
    regex r("(Speed:\t\d*)");
    smatch sm;
    while(regex_search(log, sm, r))
    {
        cout << sm.str() << '\n';
        cout << sm.prefix() <<endl;
        log = sm.suffix();
    }
 
    
}

void test_regex_replace()
{
    string text = "Quick brown fox";
    regex vowel_re("o");

      // 写结果到输出迭代器
    regex_replace(ostreambuf_iterator<char>(cout),
                  text.begin(), text.end(), vowel_re, string("*"));

      // 构造保有结果的字符串
    cout << '\n' << regex_replace(text, vowel_re, string(" ")) << '\n';
}

void test_regex()
{
   // test_regex_match();
   // test_regex_search();
   // test_regex_replace();

    // 是否匹配整个序列，第一个参数为被匹配的str，第二个参数为regex对象
    cout << regex_match("123", regex("[0-9]+")) << endl;

    // regex搜索
    string str = "subject";
    regex re("(sub)(.*)");
    smatch sm;   // 存放string结果的容器
    regex_match(str, sm, re);
    for(int i = 0; i < sm.size(); ++i)
        cout << sm[i] << " ";
    cout << endl;

    // regex搜索多次
    str = "!!!123!!!12333!!!890!!!";
    re = regex("[0-9]+");
    while(regex_search(str, sm, re)) {
        for(int i = 0; i < sm.size(); ++i)
            cout << sm[i] << " ";
        cout << endl;
        str = sm.suffix().str();
    }

}

#endif 
