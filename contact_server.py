import socket
import json
from datetime import datetime

# 서버 설정
HOST = '127.0.0.1'
PORT = 9999

# 메모리 저장소 (연락처 리스트)
contacts = [
    {"name": "관리자", "phone": "010-0000-0000", "email": "admin@example.com"}
]

def handle_request(raw_data):
    global contacts
    try:
        request = json.loads(raw_data)
        command = request.get("command")
        params = request.get("params", {})

        if command == "TIME":
            now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            return {"status": "success", "data": f"현재 시간: {now}"}

        elif command == "LIST":
            return {"status": "success", "data": contacts}

        elif command == "ADD":
            new_contact = {
                "name": params.get("name"),
                "phone": params.get("phone"),
                "email": params.get("email")
            }
            contacts.append(new_contact)
            return {"status": "success", "message": f"'{new_contact['name']}' 연락처가 등록되었습니다."}

        elif command == "DELETE":
            name_to_delete = params.get("name")
            original_count = len(contacts)
            contacts = [c for c in contacts if c['name'] != name_to_delete]
            
            if len(contacts) < original_count:
                return {"status": "success", "message": f"'{name_to_delete}' 연락처가 삭제되었습니다."}
            else:
                return {"status": "error", "message": "해당 이름의 연락처를 찾을 수 없습니다."}

        else:
            return {"status": "error", "message": "알 수 없는 명령입니다."}

    except Exception as e:
        return {"status": "error", "message": str(e)}

def start_server():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind((HOST, PORT))
    server_socket.listen()
    print(f"연락처 관리 서버가 {PORT} 포트에서 대기 중입니다...")

    while True:
        client_socket, addr = server_socket.accept()
        try:
            data = client_socket.recv(4096).decode('utf-8')
            if not data: continue
            
            response = handle_request(data)
            client_socket.sendall(json.dumps(response, ensure_ascii=False).encode('utf-8'))
        finally:
            client_socket.close()

if __name__ == "__main__":
    start_server()
