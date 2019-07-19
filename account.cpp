//hi        : allowance에 등록된 계정만 액션 실행
//insert    : allowance에 계정 등록, 컨트랙트 배포자만 실행 가능
//delete    : allowance에 계정 삭제, 컨트랙트 배포자만 실행 가능
//scope     : get_self().value
// table  
//     allowance{
//         name user;
//         uint64_t primary_key() const{
//             return user.value;
//         }
//     }

#include <eosio/eosio.hpp>
using namespace eosio;

CONTRACT account: public contract {
public:
using contract::contract;



ACTION insert(name user) {


    check(has_auth(get_self()),"no allowance");
        allowance_index forInsert(get_self(), get_self().value);
        auto itr = forInsert.find(user.value);
        
        check(itr == forInsert.end(), "already exists");
        forInsert.emplace(get_self(), [&](auto& row) {
        row.user = user;
    });

    print("insert success");
    }


ACTION remove(name user) {
    require_auth(user);

    check(has_auth(get_self()),"no allowance");
        
        allowance_index forRemove(get_self(), get_self().value);
        auto itr = forRemove.require_find(user.value, "no account");
        forRemove.erase(itr);

        print("remove success");

}


ACTION hi(name user){
        require_auth(user);
        
        allowance_index forHi(get_self(), get_self().value);
        auto itr = forHi.find(user.value);

        if(itr != forHi.end()){
            check(has_auth(user) ,"xxxxx");
            print("hi,",user);
        }
        else{
            print("no allowance");
            }
}


private:
struct [[eosio::table]] allowance {
    name user;

    uint64_t primary_key() const { return user.value; }
    };
    
    typedef multi_index<"allowance"_n, allowance> allowance_index;
};