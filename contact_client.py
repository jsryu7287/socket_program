import socket
import json

HOST = '127.0.0.1'
PORT = 9999

def send_command(command, params=None):
    try:
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect((HOST, PORT))
        
        request = {"command": command, "params": params or {}}
        client_socket.sendall(json.dumps(request).encode('utf-8'))
        
        response_data = client_socket.recv(4096).decode('utf-8')
        return json.loads(response_data)
    except Exception as e:
        return {"status": "error", "message": f"연결 실패: {e}"}
    finally:
        client_socket.close()

def main_menu():
    while True:
        print("\n--- 연락처 관리 시스템 ---")
        print("1. 날짜/시간 표시")
        print("2. 연락처 모두 표시")
        print("3. 연락처 등록")
        print("4. 연락처 삭제")
        print("5. 종료")
        choice = input("메뉴 선택: ")

        if choice == '1':
            res = send_command("TIME")
            print(f"\n[결과] {res.get('data')}")

        elif choice == '2':
            res = send_command("LIST")
            print("\n[연락처 목록]")
            data = res.get('data', [])
            if not data:
                print("등록된 연락처가 없습니다.")
            else:
                for idx, c in enumerate(data, 1):
                    print(f"{idx}. 이름: {c['name']}, 전화: {c['phone']}, 이메일: {c['email']}")

        elif choice == '3':
            print("\n[새 연락처 등록]")
            name = input("이름: ")
            phone = input("전화번호: ")
            email = input("이메일: ")
            res = send_command("ADD", {"name": name, "phone": phone, "email": email})
            print(f"\n[결과] {res.get('message')}")

        elif choice == '4':
            name = input("\n삭제할 이름 입력: ")
            res = send_command("DELETE", {"name": name})
            print(f"\n[결과] {res.get('message')}")

        elif choice == '5':
            print("프로그램을 종료합니다.")
            break
        else:
            print("잘못된 선택입니다. 다시 입력해 주세요.")

if __name__ == "__main__":
    main_menu()
