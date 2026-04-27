<div align="center">

# 🖥️ HotKeyHub Host

**Серверная часть для [HotKeyHub](https://www.rustore.ru/catalog/app/org.qtproject.hotkeyapp) — принимает команды с телефона и выполняет их на ПК**

![Platform](https://img.shields.io/badge/Платформа-Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![Stack](https://img.shields.io/badge/Стек-C%2B%2B%20%7C%20Boost-00599C?style=for-the-badge)
![Type](https://img.shields.io/badge/Тип-Консольное_приложение-555?style=for-the-badge)

</div>

---

## 📱 Android-приложение

Это серверная часть. Само приложение для телефона скачивается здесь:

[![RuStore](https://img.shields.io/badge/RuStore-Скачать_HotKeyHub-00D97E?style=for-the-badge&logo=android&logoColor=white)](https://www.rustore.ru/catalog/app/org.qtproject.hotkeyapp)

---

## 💡 Как это работает

```
[Телефон с HotKeyHub] ──Wi-Fi──▶ [HotKeyHub Host на ПК] ──▶ [Действие на ПК]
```

1. Запускаете `HotKeyHub Host` на ПК — он показывает локальный IP-адрес
2. Вводите этот адрес в приложении на телефоне
3. Телефон подключается — в консоли появляется уведомление
4. Теперь телефон управляет ПК: клавиши, мышь, медиа

---

## 🚀 Запуск

1. Скачайте `HotKeyHubHost.exe` со страницы [Releases](../../releases)
2. Запустите — в консоли появится ваш локальный адрес:

```
[HotKeyHub Host] Server is running on port - 5284
[HotKeyHub Host] Connect your client to this IP - 10.247.66.80
```

3. Введите этот адрес в приложении на телефоне и нажмите «Подключиться»

> ⚠️ Телефон и ПК должны быть в **одной Wi-Fi сети**

---

## 🛠️ Стек технологий

| Компонент | Технология |
|---|---|
| Язык | C++ |
| Сеть | Boost.Asio |
| Сборка | CMake |
| Платформа | Windows |

---

## 🔗 Ссылки

- 📱 [HotKeyHub на RuStore](https://www.rustore.ru/catalog/app/org.qtproject.hotkeyapp)
