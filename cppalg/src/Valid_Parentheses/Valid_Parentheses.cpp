#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

class Solution {
public:
    bool isValid(std::string s) {
        if (!s.size()){
            return true;
        }

        std::unordered_map<char,char> SYMBOLS = {
            {'(',')'},
            {'{','}'}, 
            {'[',']'}, 
        };
       
        std::vector<char> left;
        
        for (char c: s){
            if (SYMBOLS.count(c)){
                left.push_back(c);
                continue;
            }

            if (left.empty()){
                return false;
            }

            if (SYMBOLS[left.back()] != c){
                return false;
            }

            left.pop_back();
        }

        if (!left.empty()){
            return false;
        }

        return true;
    }
};

void test(const std::string& q){
    Solution sol = Solution();

    if (sol.isValid(q)){
        std::cout<< q << " is a valid string" << std::endl;
    } else {
        std::cout<< q << " is a SHITTY string" << std::endl;
    }
}

int main() {
    std::string q = "";
    test(q);
    q = "()[]{}";
    test(q);
    q = "([])";
    test(q);
    q = "(";
    test(q);
    q = ")";
    test(q);
    q = "()";
    test(q);
    q = "(){}[[]]";
    test(q);
    q = "({}[[]]";
    test(q);
    q = "()as{}";
    test(q);
    return 0;
}
