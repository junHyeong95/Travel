#include "travelDB.h"
#include <iostream>
using namespace std;

class Mokpo : public Travel
{
private:
    char name[100];
    char area[100];
    char hotel[100];
    int num=0;
public:
    void MokpoMenu()
    {
        while(1)
        {
            system("clear");
            cout<<"==========================="<<endl;
            cout<<"    목포 신나는 여행(서버)  "<<endl;
            cout<<"==========================="<<endl;
            cout<<"      1.회원 정보 수정      "<<endl;
            cout<<"      2.회원 정보 조회      "<<endl;
            cout<<"      3.탈퇴 회원 조회      "<<endl;
            cout<<"      4.숙박 정보           "<<endl;
            cout<<"      5.관광 내역           "<<endl;
            cout<<"      6.가이드              "<<endl;
            cout<<"      0.종료                "<<endl;
            cout<<"==========================="<<endl;
            cout<<"메뉴선택 >>";
            cin>>num;
            if(num >= 0 && num <= 6)
            {
                if(num==1)
                {
                    cin.ignore(10, '\n');
                    cout<<"수정하고자하는 아이디를 입력해주세요...\n>>";
                    cin.getline(name, 100, '\n');
                    Members(name);
                }
                else if(num==2)
                {
                    Memberlist();
                }
                else if(num==3)
                {
                    deleteMemberList();
                }
                else if(num==4)
                {
                    cout<<"숙박업소 추천받기는 1번 숙박 예약 및 변경은 2번을 입력해주세요..\n>>";
                    cin>>num;
                    if(num==1)
                    {
                        GoodHotel();
                    }
                    else if(num==2)
                    {
                        cin.ignore(10, '\n');
                        cout<<"숙박하고자 하는 호텔을 입력해주세요...\n>>";
                        cin.getline(hotel, 100, '\n');
                        cout<<"예약자의 아이디을 입력해주세요...\n>>";
                        cin>>name;
                        Hotels(hotel, name);
                    }
                    else if(num==3)
                    {
                        continue;
                    }
                }
                else if(num==5)
                {
                    cout<<"여행지 예약 및 변경은 1번을 예약조회는 2번을 입력해주세요..\n>>";
                    cin>>num;
                    if(num==1)
                    {
                        cin.ignore(10, '\n');
                        cout<<"여행하고자 하는 여행지를 입력해주세요...\n>>";
                        cin.getline(name, 100, '\n');
                        TravelReservation(name); 
                    }
                    else if(num==2)
                    {
                        cin.ignore(10, '\n');
                        cout<<"조회 하고자 하는 여행지를 입력해주세요...\n>>";
                        cin.getline(name, 100, '\n');
                        TravelList(name);
                    }
                    else if(num==3)
                    {
                        continue;
                    }
                }
                else if(num==6)
                {
                    cin.ignore(10, '\n');
                    cout<<"가이드를 요청 하고자 하는  아이디를 입력해주세요...\n>>";
                    cin.getline(name, 100, '\n');
                    Guide(name);
                }
                else if(num==0)
                {
                    break;
                }
            }
            else
            {
                throw num;
            }
        }
    }
};

int main()
{
    Mokpo admin;
    
    try
    {
        admin.MokpoMenu();
    }
    catch(string choice)
    {
        cout<<choice<<" 가 입력되었습니다."<<endl;
        cout<<"잘못입력 다시 프로그램을 실행해주세요.."<<endl;
    }
    catch(int num)
    {
        cout<<num<<" 입력하셔서 "<<"1~6번 범위가 초과 됬습니다."<<endl;
        cout<<"다시 프로그램을 실행해주세요.."<<endl;
    }
    return 0;
}