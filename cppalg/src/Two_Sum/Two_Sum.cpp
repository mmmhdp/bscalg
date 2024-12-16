#include <exception>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

class Solution {
public:
    std::vector<int> twoSum(std::vector<int>& nums, int target) {
        std::unordered_map<int , int> map;

        for (int idx{0}; idx < nums.size(); idx++){
            map[target - nums[idx]] = idx;
        }

        for (int idx{0}; idx <nums.size(); idx++){
            int el = nums[idx];

            if (map.count(el) && map[el]!=idx){
                std::vector<int >  res = {idx, map[el]};
                return res;
            }
        }
        
        std::vector<int >  res {-1, -1};
        return res;
    }
};

void test(
    Solution sol,
    std::vector<int>& nums,
    int target,
    std::vector<int>& corr_ans,
    int testn
){
    try{
        std::unordered_set<int> tmp_corr_ans (corr_ans.begin(),corr_ans.end());
        std::vector<int> res = sol.twoSum(nums, target);
        std::unordered_set<int> ans (res.begin(), res.end()) ;
        if (tmp_corr_ans != ans){
            throw std::invalid_argument("incorrect answer");
        }
    } catch (const std::exception& e){
            std::cerr << "Caught: " << e.what() << std::endl;
    }
    std::cout<< "Test " <<testn << " succeed" <<std::endl;
}

int main() {

    Solution sol;

    int                 testn = 1;
    std::vector<int>    nums {2, 7, 11, 15};
    int                 target = 9;
    std::vector<int>    corr_ans {0,1};
    test(sol, nums, target, corr_ans, testn);

    testn = 2;
    nums = {3,2,4};
    target = 6;
    corr_ans = {1,2};
    test(sol, nums, target, corr_ans, testn);

    testn = 3;
    nums = {3,3};
    target = 6;
    corr_ans = {0,1};
    test(sol, nums, target, corr_ans, testn);

    return 0;
}
