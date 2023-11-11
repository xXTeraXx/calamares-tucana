from libcalamares.utils import check_target_env_call
import os
import shutil

class TucanaBootstrap:
    def run(self):
        # Define the Tucana repository URL and file names
        tucana_repo_url = "https://github.com/xXTeraXx/Tucana.git"
        tucana_scripts = ["mercury-install", "mercury-sync"]

        # I don't know what the default path is or how to get it, hopefully it's mnt?
        calamares_chroot_path = "/mnt"

        # Clone the Tucana repository
        self.clone_tucana_repo(tucana_repo_url)

        # Update INSTALL_PATH variable in the scripts
        self.update_install_path(tucana_scripts, calamares_chroot_path)

        # Run mercury-sync and mercury install commands
        self.run_mercury(calamares_chroot_path)

    def clone_tucana_repo(self, repo_url):
        # Clone Tucana repository
        check_target_env_call(['git', 'clone', repo_url, '/tmp/Tucana'])

    def update_install_path(self, scripts, chroot_path):
        # Update INSTALL_PATH variable in Tucana scripts
        for script in scripts:
            script_path = os.path.join(chroot_path, script)
            with open(script_path, 'r') as f:
                script_content = f.read()
                updated_content = script_content.replace("INSTALL_PATH=/mnt", f"INSTALL_PATH={chroot_path}")
            with open(script_path, 'w') as f:
                f.write(updated_content)

    def run_mercury(self, chroot_path):
        # Run mercury-sync and mercury install commands
        check_target_env_call(['/bin/bash', '-c', f'cd /tmp/Tucana/mercury && ./mercury-sync'])
        check_target_env_call(['/bin/bash', '-c', f'cd /tmp/Tucana/mercury && printf "y\n" | ./mercury-install base'])
