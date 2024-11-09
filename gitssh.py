import subprocess
import os
import pyperclip


def generate_ssh_key(key_name="id_ed25519 ", passphrase=""):
    # 生成 SSH 密钥
    keygen_command = [
        "ssh-keygen",
        "-t", "ed25519",
        "-C", "guoyawen98@gmail.com",
        "-f",
        os.path.expanduser(f"~/.ssh/{key_name}"),
        "-N",
        passphrase,
    ]
    subprocess.run(keygen_command, check=True)

    # 将公钥内容复制到剪贴板
    pub_key_path = os.path.expanduser(f"~/.ssh/{key_name}.pub")
    with open(pub_key_path, 'r') as pub_key_file:
        pub_key_content = pub_key_file.read()
    pyperclip.copy(pub_key_content)
    print("Public key copied to clipboard.")

    # 启动 ssh-agent 并捕获输出
    result = subprocess.run(["ssh-agent"], capture_output=True, text=True, check=True)
    output = result.stdout
    
    # print result
    print(output)

    # 将生成的密钥添加到 ssh-agent
    add_key_command = ["ssh-add", os.path.expanduser(f"~/.ssh/{key_name}")]
    subprocess.run(add_key_command, check=True)



if __name__ == "__main__":
    generate_ssh_key()
