# ft_irc

![ft_irc Miniature](ft_irc.png)

## 💬 Description

ft_irc est une implémentation d'un serveur IRC (Internet Relay Chat) en C++ 98. 
Ce projet permet de comprendre les mécanismes de communication réseau en temps réel et la gestion des protocoles de communication. 
Conforme au RFC 1459 et aux standards modernes IRC.

## ⚙️ Fonctionnalités

- Serveur IRC complet compatible avec clients standards
- Gestion des utilisateurs et authentification
- Création et gestion de canaux (channels)
- Communication publique et privée entre utilisateurs
- Commandes opérateur (op) dans les canaux
- Transfert de fichiers
- Gestion des modes utilisateur et canal

## 🛠️ Technologies utilisées

- Langage: C++98
- Architecture: Non-bloquante avec I/O multiplexing (select/poll/epoll)
- Pas de bibliothèques externes autorisées
- Testé et compatible avec Hexchat comme client IRC

## 🚀 Installation

```bash
# Cloner le dépôt
git clone https://github.com/Freiko789/ft_irc.git

# Se déplacer dans le dossier
cd ft_irc

# Compiler le projet
make

# Lancer le serveur
./ircserv <port> <password>
```

## 📡 Utilisation

```bash
# Lancement du serveur
./ircserv 6667 server_password

# Connexion avec Hexchat (client recommandé)
# Dans Hexchat, sélectionnez "Add" dans le Network List, puis configurez:
# - Nom du réseau: ft_irc
# - Serveur: localhost/6667
# - Mot de passe: server_password

# Connexion avec un autre client IRC (exemple avec irssi)
irssi -c localhost -p 6667 -w server_password
```

## 🔧 Commandes IRC supportées

- `/join #channel` - Rejoindre un canal
- `/part #channel` - Quitter un canal
- `/msg nickname message` - Envoyer un message privé
- `/nick new_nickname` - Changer de pseudo
- `/kick #channel nickname` - Expulser un utilisateur (op requis)
- `/mode #channel +o|-o nickname` - Donner/retirer les droits d'opérateur
- `/quit [message]` - Quitter le serveur

## 📝 Normes du projet

- Code conforme à la Norme 42
- Compilation avec flags: `-Wall -Wextra -Werror -std=c++98`
- Gestion des erreurs robuste
- Pas de fuites mémoire ni de comportements indéfinis

## 👥 Auteurs

- [dtassel](https://github.com/Freiko789)
- [phudyka](https://github.com/phudyka)

---
*Projet réalisé dans le cadre du cursus réseau à l'école 42*
