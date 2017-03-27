# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
  config.vm.box = "ubuntu/xenial64"

  config.vm.provider "virtualbox" do |vb|
    # override default UART settings
    vb.customize [ "modifyvm", :id, "--uartmode1", "disconnected" ]

    vb.cpus = 2
    vb.memory = 2048
  end

  config.vm.synced_folder ".", "/vagrant", disabled: true
  config.vm.synced_folder "demo", "/root/demo", type: "rsync",
    rsync__auto: false, create: true, owner: "root", group: "root"

  config.vm.provision "shell", inline: <<-SHELL
    set -e
    apt-get update
    apt-get dist-upgrade -y
    apt-get install -y build-essential coreutils util-linux python3 \
                       vim tmux htop tree busybox-static
    if ! grep -F /root/bin /root/.profile > /dev/null 2>&1; then
      mkdir -p /root/bin
      echo 'PATH=/root/bin:$PATH' >> /root/.profile
    fi
    if ! grep -F 'PS1="\\n\\u@\\h:\\w\\n\\$? \\$ "' /root/.bashrc > /dev/null 2>&1; then
      echo 'PS1="\\n\\u@\\h:\\w\\n\\$? \\$ "' >> /root/.bashrc
    fi
    if [[ -f /var/run/reboot-required ]]; then
      echo REBOOT!!
      ( (sleep 5 && reboot) & )
    fi
  SHELL

end
