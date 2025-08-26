#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
using namespace std;

// PotionRecipe 클래스: 재료 목록을 vector<string>으로 변경
class PotionRecipe 
{
public:
    string potionName;
    vector<string> ingredients; // 단일 재료에서 재료 '목록'으로 변경

    // 생성자: 재료 목록을 받아 초기화하도록 수정
    PotionRecipe(const string& name, const vector<string>& ingredients)
        : potionName(name), ingredients(ingredients) {
    }
};

//도전기능 재고 관리
class StockManager
{
private:
    const int Max_stock = 3;
    unordered_map<string, int> stockMap;

public:
    string potionStock;


    void intiatializeStock(const string potionName)
    {
        //재고 초기화
        stockMap[potionName] = Max_stock;
        std::cout << ">> '" << potionName << "' 재고가 초기화되었습니다. (최대 " << Max_stock << "개)" << std::endl;
    }

    bool dispensePotion(const string& potionName)
    {
        auto it = stockMap.find(potionName);
        if (it != stockMap.end() && it->second > 0)
        {
            it->second--;
            cout << ">> '" << potionName << "' 물약이 지급되었습니다. (남은 재고: " << it->second << ")" << endl;
            return true;
        }
        else
        {
            cout << ">> '" << potionName << "' 재고가 부족하거나 존재하지 않습니다." << endl;
            return false;
        }
    }
    void returnPotion(const string& potionName)
    {
        auto it = stockMap.find(potionName);
        if (it != stockMap.end())
        {
            if (it->second < Max_stock)
            {
                it->second++;
                std::cout << ">> '" << potionName << "' 공병이 회수되어 재고가 복구되었습니다. (현재 재고: " << it->second << ")" << std::endl;
            }
            else
            {
                std::cout << ">> '" << potionName << "' 재고가 이미 최대치입니다. 회수 불가." << std::endl;
            }
        }
        else
        {
            std::cout << ">> '" << potionName << "' 재고 정보가 없습니다. 먼저 초기화해주세요." << std::endl;
        }

    }
    int getStock(const string& potionName) const
    {
        auto it = stockMap.find(potionName);
        if (it != stockMap.end())
        {
            return it->second;
        }
        else
        {
            cout << ">> '" << potionName << "' 재고 정보가 없습니다." << endl;
            return -1; // 오류 표시
        }

    }
};

// AlchemyWorkshop 클래스: 레시피 목록을 관리
class AlchemyWorkshop
{
private:
    vector<PotionRecipe> recipes;
    StockManager& stockManager; // StockManager

public:
    AlchemyWorkshop(StockManager& stockManager) : stockManager(stockManager) {}

    // 모든 레시피 출력 메서드
    void displayAllRecipes() const 
    {
        if (recipes.empty()) 
        {
            cout << "아직 등록된 레시피가 없습니다." << endl;
            return;
        }

        cout << "\n--- [ 전체 레시피 목록 ] ---" << endl;
        for (size_t i = 0; i < recipes.size(); ++i) 
        {
            cout << "- 물약 이름: " << recipes[i].potionName << endl;
            cout << "  > 필요 재료: ";
            // 재료 목록을 순회하며 출력
            for (size_t j = 0; j < recipes[i].ingredients.size(); ++j) 
            {
                cout << recipes[i].ingredients[j];
                // 마지막 재료가 아니면 쉼표로 구분
                if (j < recipes[i].ingredients.size() - 1) 
                {
                    cout << ", ";
                }
            }
            cout << endl;
            int stock = stockManager.getStock(recipes[i].potionName);
            cout << "[재고 확인] '" << recipes[i].potionName << "' 현재 재고 : " << stock << "개" << endl;
        }
        cout << "---------------------------\n";
    }
    void setRecipes(const vector<PotionRecipe>& newRecipes)
    {
        recipes = newRecipes;
    }
};


// 도전기능 레시피 관리
class RecipeManager
{
private:
    vector<PotionRecipe> recipes;
    StockManager& stockManager;

public:
    RecipeManager(StockManager& stockManager) : stockManager(stockManager) {}

    void addRecipe(const string& name, const vector<string>& ingredients)
    {
        recipes.push_back(PotionRecipe(name, ingredients));
        stockManager.intiatializeStock(name);
        cout << ">> 새로운 레시피 '" << name << "'이(가) 추가되었습니다." << endl;
    }
    void findRecipeByName(const string& name)
    {
        auto it = find_if(recipes.begin(), recipes.end(), [&name](const PotionRecipe& recipe) {return recipe.potionName == name; });
        cout << " ------[검색결과]------ " << endl;
        if (it != recipes.end())
        {
            cout << "- 물약 이름: " << it->potionName << endl;
            cout << "  > 필요 재료: ";

            // 재료 목록을 출력
            for (size_t k = 0; k < it->ingredients.size(); ++k)
            {
                cout << it->ingredients[k];
                // 마지막 재료가 아니면 쉼표로 구분
                if (k < it->ingredients.size() - 1)
                {
                    cout << ", ";
                }
            }
            cout << endl;
        }
        else
        {
            cout << "찾는 물약이 없습니다." << endl;
        }
        cout << "---------------------------\n";
    }

    //재료로 물약 검색
    void findRecipeByIngrdient(const string& ingredient)
    {
        cout << " ------[검색결과]------ " << endl;
        bool found = false;

        for (const auto& recipe : recipes)
        {
            // 검색한 재료가 포함되어 있는지 확인
            if (find(recipe.ingredients.begin(), recipe.ingredients.end(), ingredient) != recipe.ingredients.end())
            {
                cout << "- 물약 이름: " << recipe.potionName << endl;
                cout << "  > 필요 재료: ";

                for (size_t k = 0; k < recipe.ingredients.size(); ++k)
                {
                    cout << recipe.ingredients[k];
                    if (k < recipe.ingredients.size() - 1)
                    {
                        cout << ", ";
                    }
                }
                cout << endl;
                found = true;
            }
        }
        if (!found)
        {
            cout << "해당 재료를 포함한 물약이 없습니다." << endl;
        }

        cout << "---------------------------\n";
    }

    // 전체 레시피를 복사해서 반환
    const vector<PotionRecipe> getAllRecipes() const
    {
        return recipes;
    }

};


int main() 
{
    StockManager stockManager;
    RecipeManager recipeManager(stockManager);
    AlchemyWorkshop myWorkshop(stockManager);

    while (true) 
    {
       
        cout << "            |   |   |\n";
        cout << "          |   |   |\n";
        cout << "         |  |  |\n";
        cout << "         | | | \n";
        cout << "     _____| |  \n";
        cout << "    /     \\   \n";
        cout << "   /       \\  \n";
        cout << "  |     ~~~ |  \n";
        cout << "  |~~~      |  \n";
        cout << "  |  o o o  |  \n";
        cout << "   \\_______/  \n";
        cout << "     |   |     \n";
        cout << "     |   |     \n";
        cout << "---------------------------------\n";
        cout << "--<연금술 공방 관리 시스템>--" << endl;
        cout << "1. 레시피 추가" << endl;
        cout << "2. 모든 레시피 출력" << endl;
        cout << "3. 레시피 물약이름으로 찾기" << endl;
        cout << "4. 레시피 재료이름으로 찾기" << endl;
        cout << "5. 종료" << endl;
        cout << "---------------------------------\n";
        cout << " 선택: ";

        int choice;
        cin >> choice;

        if (cin.fail()) {
            cout << "잘못된 입력입니다. 숫자를 입력해주세요." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 1) {
            string name;
            cout << "물약 이름: ";
            cin.ignore(10000, '\n');
            getline(cin, name);

            // 여러 재료를 입력받기 위한 로직
            vector<string> ingredients_input;
            string ingredient;
            cout << "필요한 재료들을 입력하세요. (입력 완료 시 '끝' 입력)" << endl;

            while (true) {
                cout << "재료 입력: ";
                getline(cin, ingredient);

                // 사용자가 '끝'을 입력하면 재료 입력 종료
                if (ingredient == "끝") {
                    break;
                }
                ingredients_input.push_back(ingredient);
            }

            // 입력받은 재료가 하나 이상 있을 때만 레시피 추가
            if (!ingredients_input.empty()) {
                recipeManager.addRecipe(name, ingredients_input);
            }
            else 
            {
                cout << ">> 재료가 입력되지 않아 레시피 추가를 취소합니다." << endl;
            }

        }
        else if (choice == 2) 
        {
            myWorkshop.setRecipes(recipeManager.getAllRecipes());
            myWorkshop.displayAllRecipes();
        }

        // 물약 이름으로 레시피 찾기
        else if (choice == 3) 
        {
            string name;
            cout << "물약 이름: ";
            cin.ignore(10000, '\n');
            getline(cin, name);
            recipeManager.findRecipeByName(name);
        }
        // 재료 이름으로 레시피 찾기
        else if (choice == 4)
        {
            string ingredient;
            cout << "재료 이름: ";
            cin.ignore(10000, '\n');
            getline(cin, ingredient);
            recipeManager.findRecipeByIngrdient(ingredient);
        }

        else if (choice == 5) {
            cout << "공방 문을 닫습니다..." << endl;
            break;

        }

        else 
        {
            cout << "잘못된 선택입니다. 다시 시도하세요." << endl;
        }
    }

    return 0;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
