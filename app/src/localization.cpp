#include "localization.hpp"

#include <algorithm>
#include <unordered_map>

namespace opennow
{
namespace
{

using Dictionary = std::unordered_map<std::string, std::string>;

std::string g_language = "en";

const Dictionary& DictionaryFor(const std::string& code)
{
    static const Dictionary empty;
    static const std::unordered_map<std::string, Dictionary> dictionaries = {
        {"ru", {
            {"Store", "Магазин"}, {"Library", "Библиотека"}, {"Settings", "Настройки"},
            {"My Library", "Моя библиотека"}, {"Search", "Поиск"}, {"Close", "Закрыть"},
            {"Cancel", "Отмена"}, {"Account", "Аккаунт"}, {"Stream", "Поток"},
            {"Game", "Игра"}, {"Controls", "Управление"}, {"Audio", "Аудио"},
            {"Storage", "Хранилище"}, {"Interface", "Интерфейс"},
            {"Language", "Язык интерфейса"}, {"Choose the launcher language.", "Выберите язык интерфейса приложения."},
            {"CATEGORIES", "КАТЕГОРИИ"}, {"All changes saved", "Все изменения сохранены"},
            {"Unsaved changes  |  X Save", "Есть изменения  |  X Сохранить"},
            {"Settings are already up to date", "Настройки уже актуальны"},
            {"Settings saved; changes apply to the next stream", "Настройки сохранены; параметры потока применятся к следующему сеансу"},
            {"Unsaved changes reverted", "Несохраненные изменения отменены"},
            {"Quality profile", "Профиль качества"}, {"Preset", "Профиль"},
            {"Resolution", "Разрешение"}, {"Frame rate", "Частота кадров"},
            {"Maximum bitrate", "Максимальный битрейт"}, {"Motion clarity", "Четкость в движении"},
            {"Video backend", "Видеодекодер"}, {"Decoder & delivery", "Декодирование и доставка"},
            {"Recovery", "Восстановление"}, {"Diagnostics", "Диагностика"},
            {"Debug diagnostics", "Отладочная диагностика"}, {"Enabled", "Включено"},
            {"Disabled", "Выключено"}, {"Muted", "Без звука"},
            {"Stream audio", "Звук трансляции"}, {"Audio output", "Вывод звука"},
            {"Volume boost", "Усиление громкости"}, {"Playback buffer", "Буфер воспроизведения"},
            {"Output format", "Формат вывода"}, {"Game preferences", "Настройки игры"},
            {"Game language", "Язык игры"}, {"Save in-game graphics settings", "Сохранять настройки графики игры"},
            {"Controller layout", "Раскладка контроллера"}, {"Face buttons", "Кнопки ABXY"},
            {"Connected account", "Подключенный аккаунт"}, {"Status", "Статус"},
            {"Not connected", "Не подключен"}, {"User", "Пользователь"},
            {"Membership", "Подписка"}, {"Provider", "Провайдер"},
            {"Authentication", "Авторизация"}, {"Quick sign-in", "Быстрый вход"},
            {"Saved accounts", "Сохраненные аккаунты"}, {"Session", "Сессия"},
            {"Session details", "Данные сессии"}, {"Refresh authorization", "Обновить авторизацию"},
            {"Choose saved account", "Выбрать сохраненный аккаунт"}, {"Forget saved passwords", "Забыть сохраненные пароли"},
            {"Account removal", "Удаление аккаунта"}, {"Remove active account", "Удалить активный аккаунт"},
            {"Remove every account", "Удалить все аккаунты"}, {"View", "Открыть"},
            {"Refresh", "Обновить"}, {"Choose", "Выбрать"}, {"Forget", "Забыть"},
            {"Remove", "Удалить"}, {"Remove all", "Удалить все"}, {"Restore", "Восстановить"},
            {"Cover cache", "Кэш обложек"}, {"Cover files", "Файлы обложек"},
            {"Disk usage", "Использование диска"}, {"Public catalog", "Публичный каталог"},
            {"Owned library", "Моя библиотека"}, {"Loaded", "Загружено"}, {"Not loaded", "Не загружено"},
            {"Inspect shared cache", "Проверить общий кэш"}, {"Clear cover artwork", "Очистить обложки"},
            {"Inspect", "Проверить"}, {"Clear", "Очистить"}, {"System", "Система"},
            {"Open diagnostics", "Открыть диагностику"}, {"Open", "Открыть"},
            {"Sign in to GeForce NOW", "Войти в GeForce NOW"}, {"Refresh library", "Обновить библиотеку"},
            {"Sign out", "Выйти"}, {"Add another account", "Добавить аккаунт"},
            {"Reconnect this account", "Переподключить аккаунт"}, {"No account", "Нет аккаунта"},
            {"Play on GeForce NOW", "Играть в GeForce NOW"}, {"Play from Store", "Играть из магазина"},
            {"In your library", "В вашей библиотеке"}, {"Supported in GeForce NOW", "Поддерживается в GeForce NOW"},
            {"Choose game store", "Выберите магазин игры"}, {"Not Logged In", "Вход не выполнен"},
            {"Launch Error", "Ошибка запуска"}, {"NOW LOADING", "ЗАПУСК"},
            {"Checking your NVIDIA account", "Проверка аккаунта NVIDIA"},
            {"Requesting a cloud rig", "Запрос облачного компьютера"},
            {"Waiting in queue...", "Ожидание в очереди..."}, {"Preparing your cloud rig", "Подготовка облачного компьютера"},
            {"Waiting for an available cloud rig", "Ожидание свободного облачного компьютера"},
            {"Session could not start", "Не удалось запустить сессию"}, {"Cancel session", "Отменить сессию"},
            {"Choose how to sign in", "Выберите способ входа"}, {"Welcome back", "С возвращением"},
            {"Connect your account", "Подключите аккаунт"}, {"NVIDIA account", "Аккаунт NVIDIA"},
            {"No saved account yet", "Сохраненного аккаунта пока нет"}, {"QUICK SIGN-IN READY", "БЫСТРЫЙ ВХОД ГОТОВ"},
            {"NEW ACCOUNT", "НОВЫЙ АККАУНТ"}, {"Use another NVIDIA account", "Использовать другой аккаунт NVIDIA"},
            {"Enter email and password", "Ввести почту и пароль"}, {"Phone / PC fallback for CAPTCHA or passkey", "Вход через телефон / ПК для CAPTCHA или ключа доступа"},
            {"SIGN-IN PREFERENCES", "НАСТРОЙКИ ВХОДА"}, {"Remember sign-in: ON", "Запоминать вход: ВКЛ"},
            {"Cancel active sign-in", "Отменить текущий вход"}, {"Approve the NVIDIA sign-in by email", "Подтвердите вход NVIDIA по электронной почте"},
            {"Cancel sign-in", "Отменить вход"}, {"Signing in directly with NVIDIA...", "Вход напрямую через NVIDIA..."},
            {"Sign-in cancelled", "Вход отменен"}, {"Waiting for NVIDIA verification", "Ожидание подтверждения NVIDIA"},
            {"Login failed", "Ошибка входа"}, {"Checking saved sign-in", "Проверка сохраненного входа"},
            {"SAVED NVIDIA ACCOUNT", "СОХРАНЕННЫЙ АККАУНТ NVIDIA"}
        }},
        {"uk", {
            {"Store", "Магазин"}, {"Library", "Бібліотека"}, {"Settings", "Налаштування"},
            {"My Library", "Моя бібліотека"}, {"Search", "Пошук"}, {"Close", "Закрити"},
            {"Cancel", "Скасувати"}, {"Account", "Обліковий запис"}, {"Stream", "Трансляція"},
            {"Game", "Гра"}, {"Controls", "Керування"}, {"Audio", "Аудіо"},
            {"Storage", "Сховище"}, {"Interface", "Інтерфейс"}, {"Language", "Мова інтерфейсу"},
            {"Choose the launcher language.", "Виберіть мову інтерфейсу застосунку."},
            {"CATEGORIES", "КАТЕГОРІЇ"}, {"All changes saved", "Усі зміни збережено"},
            {"Unsaved changes  |  X Save", "Є зміни  |  X Зберегти"},
            {"Quality profile", "Профіль якості"}, {"Preset", "Профіль"}, {"Resolution", "Роздільна здатність"},
            {"Frame rate", "Частота кадрів"}, {"Maximum bitrate", "Максимальний бітрейт"},
            {"Diagnostics", "Діагностика"}, {"Enabled", "Увімкнено"}, {"Disabled", "Вимкнено"},
            {"Stream audio", "Звук трансляції"}, {"Volume boost", "Підсилення гучності"},
            {"Game preferences", "Налаштування гри"}, {"Game language", "Мова гри"},
            {"Controller layout", "Розкладка контролера"}, {"Face buttons", "Кнопки ABXY"},
            {"Connected account", "Підключений обліковий запис"}, {"Status", "Стан"},
            {"Not connected", "Не підключено"}, {"User", "Користувач"}, {"Membership", "Підписка"},
            {"Authentication", "Авторизація"}, {"Quick sign-in", "Швидкий вхід"},
            {"Saved accounts", "Збережені облікові записи"}, {"Session", "Сеанс"},
            {"Refresh", "Оновити"}, {"Choose", "Вибрати"}, {"Remove", "Видалити"},
            {"Cover cache", "Кеш обкладинок"}, {"System", "Система"},
            {"Sign in to GeForce NOW", "Увійти в GeForce NOW"}, {"Refresh library", "Оновити бібліотеку"},
            {"Sign out", "Вийти"}, {"Play on GeForce NOW", "Грати в GeForce NOW"},
            {"In your library", "У вашій бібліотеці"}, {"NOW LOADING", "ЗАПУСК"},
            {"Checking your NVIDIA account", "Перевірка облікового запису NVIDIA"},
            {"Waiting in queue...", "Очікування в черзі..."}, {"Preparing your cloud rig", "Підготовка хмарного комп'ютера"},
            {"Cancel session", "Скасувати сеанс"}, {"Choose how to sign in", "Виберіть спосіб входу"},
            {"Welcome back", "З поверненням"}, {"Use another NVIDIA account", "Інший обліковий запис NVIDIA"},
            {"Enter email and password", "Ввести пошту та пароль"}, {"Login failed", "Помилка входу"}
        }},
        {"es", {
            {"Store", "Tienda"}, {"Library", "Biblioteca"}, {"Settings", "Ajustes"}, {"My Library", "Mi biblioteca"},
            {"Search", "Buscar"}, {"Close", "Cerrar"}, {"Cancel", "Cancelar"}, {"Account", "Cuenta"},
            {"Stream", "Transmisión"}, {"Game", "Juego"}, {"Controls", "Controles"}, {"Audio", "Audio"},
            {"Storage", "Almacenamiento"}, {"Interface", "Interfaz"}, {"Language", "Idioma de la interfaz"},
            {"Choose the launcher language.", "Elige el idioma de la aplicación."}, {"CATEGORIES", "CATEGORÍAS"},
            {"All changes saved", "Todos los cambios guardados"}, {"Unsaved changes  |  X Save", "Cambios sin guardar  |  X Guardar"},
            {"Quality profile", "Perfil de calidad"}, {"Preset", "Preajuste"}, {"Resolution", "Resolución"},
            {"Frame rate", "Frecuencia de imagen"}, {"Maximum bitrate", "Bitrate máximo"},
            {"Diagnostics", "Diagnóstico"}, {"Enabled", "Activado"}, {"Disabled", "Desactivado"},
            {"Stream audio", "Audio de transmisión"}, {"Volume boost", "Amplificación de volumen"},
            {"Game preferences", "Preferencias del juego"}, {"Game language", "Idioma del juego"},
            {"Controller layout", "Distribución del mando"}, {"Face buttons", "Botones ABXY"},
            {"Connected account", "Cuenta conectada"}, {"Status", "Estado"}, {"Not connected", "Sin conexión"},
            {"Quick sign-in", "Inicio rápido"}, {"Saved accounts", "Cuentas guardadas"},
            {"Refresh", "Actualizar"}, {"Choose", "Elegir"}, {"Remove", "Eliminar"},
            {"Sign in to GeForce NOW", "Iniciar sesión en GeForce NOW"}, {"Refresh library", "Actualizar biblioteca"},
            {"Sign out", "Cerrar sesión"}, {"Play on GeForce NOW", "Jugar en GeForce NOW"},
            {"In your library", "En tu biblioteca"}, {"NOW LOADING", "CARGANDO"},
            {"Checking your NVIDIA account", "Comprobando tu cuenta NVIDIA"}, {"Waiting in queue...", "Esperando en la cola..."},
            {"Preparing your cloud rig", "Preparando tu equipo en la nube"}, {"Cancel session", "Cancelar sesión"},
            {"Choose how to sign in", "Elige cómo iniciar sesión"}, {"Welcome back", "Bienvenido de nuevo"},
            {"Enter email and password", "Introducir correo y contraseña"}, {"Login failed", "Error de inicio de sesión"}
        }},
        {"it", {
            {"Store", "Negozio"}, {"Library", "Libreria"}, {"Settings", "Impostazioni"}, {"My Library", "La mia libreria"},
            {"Search", "Cerca"}, {"Close", "Chiudi"}, {"Cancel", "Annulla"}, {"Account", "Account"},
            {"Stream", "Streaming"}, {"Game", "Gioco"}, {"Controls", "Comandi"}, {"Audio", "Audio"},
            {"Storage", "Archiviazione"}, {"Interface", "Interfaccia"}, {"Language", "Lingua dell'interfaccia"},
            {"Choose the launcher language.", "Scegli la lingua dell'applicazione."}, {"CATEGORIES", "CATEGORIE"},
            {"All changes saved", "Tutte le modifiche sono salvate"}, {"Unsaved changes  |  X Save", "Modifiche non salvate  |  X Salva"},
            {"Quality profile", "Profilo qualità"}, {"Preset", "Profilo"}, {"Resolution", "Risoluzione"},
            {"Frame rate", "Frequenza fotogrammi"}, {"Maximum bitrate", "Bitrate massimo"},
            {"Diagnostics", "Diagnostica"}, {"Enabled", "Attivo"}, {"Disabled", "Disattivato"},
            {"Game language", "Lingua del gioco"}, {"Controller layout", "Layout controller"},
            {"Connected account", "Account collegato"}, {"Quick sign-in", "Accesso rapido"},
            {"Refresh", "Aggiorna"}, {"Choose", "Scegli"}, {"Remove", "Rimuovi"},
            {"Sign in to GeForce NOW", "Accedi a GeForce NOW"}, {"Refresh library", "Aggiorna libreria"},
            {"Sign out", "Esci"}, {"Play on GeForce NOW", "Gioca su GeForce NOW"}, {"In your library", "Nella tua libreria"},
            {"NOW LOADING", "CARICAMENTO"}, {"Checking your NVIDIA account", "Verifica dell'account NVIDIA"},
            {"Waiting in queue...", "In attesa in coda..."}, {"Preparing your cloud rig", "Preparazione del PC cloud"},
            {"Cancel session", "Annulla sessione"}, {"Choose how to sign in", "Scegli come accedere"},
            {"Welcome back", "Bentornato"}, {"Login failed", "Accesso non riuscito"}
        }},
        {"fr", {
            {"Store", "Boutique"}, {"Library", "Bibliothèque"}, {"Settings", "Paramètres"}, {"My Library", "Ma bibliothèque"},
            {"Search", "Rechercher"}, {"Close", "Fermer"}, {"Cancel", "Annuler"}, {"Account", "Compte"},
            {"Stream", "Streaming"}, {"Game", "Jeu"}, {"Controls", "Commandes"}, {"Audio", "Audio"},
            {"Storage", "Stockage"}, {"Interface", "Interface"}, {"Language", "Langue de l'interface"},
            {"Choose the launcher language.", "Choisissez la langue de l'application."}, {"CATEGORIES", "CATÉGORIES"},
            {"All changes saved", "Toutes les modifications sont enregistrées"}, {"Unsaved changes  |  X Save", "Modifications non enregistrées  |  X Enregistrer"},
            {"Quality profile", "Profil de qualité"}, {"Preset", "Préréglage"}, {"Resolution", "Résolution"},
            {"Frame rate", "Fréquence d'images"}, {"Maximum bitrate", "Débit maximal"},
            {"Diagnostics", "Diagnostic"}, {"Enabled", "Activé"}, {"Disabled", "Désactivé"},
            {"Stream audio", "Audio du streaming"}, {"Game language", "Langue du jeu"},
            {"Controller layout", "Disposition de la manette"}, {"Connected account", "Compte connecté"},
            {"Quick sign-in", "Connexion rapide"}, {"Refresh", "Actualiser"}, {"Choose", "Choisir"}, {"Remove", "Supprimer"},
            {"Sign in to GeForce NOW", "Se connecter à GeForce NOW"}, {"Refresh library", "Actualiser la bibliothèque"},
            {"Sign out", "Se déconnecter"}, {"Play on GeForce NOW", "Jouer sur GeForce NOW"}, {"In your library", "Dans votre bibliothèque"},
            {"NOW LOADING", "CHARGEMENT"}, {"Checking your NVIDIA account", "Vérification de votre compte NVIDIA"},
            {"Waiting in queue...", "En attente dans la file..."}, {"Preparing your cloud rig", "Préparation de votre machine cloud"},
            {"Cancel session", "Annuler la session"}, {"Choose how to sign in", "Choisissez comment vous connecter"},
            {"Welcome back", "Bon retour"}, {"Login failed", "Échec de la connexion"}
        }},
        {"pl", {
            {"Store", "Sklep"}, {"Library", "Biblioteka"}, {"Settings", "Ustawienia"}, {"My Library", "Moja biblioteka"},
            {"Search", "Szukaj"}, {"Close", "Zamknij"}, {"Cancel", "Anuluj"}, {"Account", "Konto"},
            {"Stream", "Strumień"}, {"Game", "Gra"}, {"Controls", "Sterowanie"}, {"Audio", "Dźwięk"},
            {"Storage", "Pamięć"}, {"Interface", "Interfejs"}, {"Language", "Język interfejsu"},
            {"Choose the launcher language.", "Wybierz język aplikacji."}, {"CATEGORIES", "KATEGORIE"},
            {"All changes saved", "Wszystkie zmiany zapisano"}, {"Unsaved changes  |  X Save", "Niezapisane zmiany  |  X Zapisz"},
            {"Quality profile", "Profil jakości"}, {"Preset", "Profil"}, {"Resolution", "Rozdzielczość"},
            {"Frame rate", "Liczba klatek"}, {"Maximum bitrate", "Maksymalny bitrate"},
            {"Diagnostics", "Diagnostyka"}, {"Enabled", "Włączone"}, {"Disabled", "Wyłączone"},
            {"Game language", "Język gry"}, {"Controller layout", "Układ kontrolera"},
            {"Connected account", "Połączone konto"}, {"Quick sign-in", "Szybkie logowanie"},
            {"Refresh", "Odśwież"}, {"Choose", "Wybierz"}, {"Remove", "Usuń"},
            {"Sign in to GeForce NOW", "Zaloguj do GeForce NOW"}, {"Refresh library", "Odśwież bibliotekę"},
            {"Sign out", "Wyloguj"}, {"Play on GeForce NOW", "Graj w GeForce NOW"}, {"In your library", "W twojej bibliotece"},
            {"NOW LOADING", "URUCHAMIANIE"}, {"Checking your NVIDIA account", "Sprawdzanie konta NVIDIA"},
            {"Waiting in queue...", "Oczekiwanie w kolejce..."}, {"Preparing your cloud rig", "Przygotowywanie komputera w chmurze"},
            {"Cancel session", "Anuluj sesję"}, {"Choose how to sign in", "Wybierz sposób logowania"},
            {"Welcome back", "Witaj ponownie"}, {"Login failed", "Logowanie nie powiodło się"}
        }},
        {"zh-CN", {
            {"Store", "商店"}, {"Library", "游戏库"}, {"Settings", "设置"}, {"My Library", "我的游戏库"},
            {"Search", "搜索"}, {"Close", "关闭"}, {"Cancel", "取消"}, {"Account", "账户"},
            {"Stream", "串流"}, {"Game", "游戏"}, {"Controls", "控制"}, {"Audio", "音频"},
            {"Storage", "存储"}, {"Interface", "界面"}, {"Language", "界面语言"},
            {"Choose the launcher language.", "选择应用界面语言。"}, {"CATEGORIES", "分类"},
            {"All changes saved", "所有更改已保存"}, {"Unsaved changes  |  X Save", "有未保存的更改  |  X 保存"},
            {"Quality profile", "画质预设"}, {"Preset", "预设"}, {"Resolution", "分辨率"},
            {"Frame rate", "帧率"}, {"Maximum bitrate", "最大码率"}, {"Diagnostics", "诊断"},
            {"Enabled", "已启用"}, {"Disabled", "已禁用"}, {"Stream audio", "串流音频"},
            {"Game language", "游戏语言"}, {"Controller layout", "手柄布局"},
            {"Connected account", "已连接账户"}, {"Quick sign-in", "快速登录"},
            {"Refresh", "刷新"}, {"Choose", "选择"}, {"Remove", "删除"},
            {"Sign in to GeForce NOW", "登录 GeForce NOW"}, {"Refresh library", "刷新游戏库"},
            {"Sign out", "退出登录"}, {"Play on GeForce NOW", "在 GeForce NOW 上游玩"}, {"In your library", "已在游戏库中"},
            {"NOW LOADING", "正在启动"}, {"Checking your NVIDIA account", "正在检查 NVIDIA 账户"},
            {"Waiting in queue...", "正在排队..."}, {"Preparing your cloud rig", "正在准备云端设备"},
            {"Cancel session", "取消会话"}, {"Choose how to sign in", "选择登录方式"},
            {"Welcome back", "欢迎回来"}, {"Login failed", "登录失败"}
        }}
    };

    const auto found = dictionaries.find(code);
    return found == dictionaries.end() ? empty : found->second;
}

const Dictionary& ServerDictionaryFor(const std::string& code)
{
    static const Dictionary empty;
    static const std::unordered_map<std::string, Dictionary> dictionaries = {
        {"ru", {
            {"Server location", "Местоположение сервера"}, {"Use automatic routing or choose a specific GeForce NOW data center.", "Используйте автоматическую маршрутизацию или выберите конкретный центр обработки данных GeForce NOW."},
            {"Location", "Сервер"}, {"Locations are ranked by direct latency from this console.", "Серверы отсортированы по прямой задержке с этой консоли."},
            {"Latency test", "Тест задержки"}, {"Reload available locations and test each connection again.", "Повторно загрузить доступные серверы и проверить каждое подключение."},
            {"Testing locations...", "Проверка серверов..."}, {"Auto (best)", "Авто (лучший)"}, {"Auto", "Авто"},
            {"Selected server", "Выбранный сервер"}, {"Current server (not advertised)", "Текущий сервер (не опубликован)"},
            {"Unavailable", "Недоступен"}, {"Best", "Лучший"}, {"Server location test is already running", "Проверка серверов уже выполняется"},
            {"Server Locations Unavailable", "Серверы недоступны"}, {"Connect a GeForce NOW account before loading server locations.", "Подключите аккаунт GeForce NOW перед загрузкой серверов."},
            {"GeForce NOW did not return any server locations.", "GeForce NOW не вернул список серверов."}, {"Server location latency test complete", "Проверка задержки серверов завершена"},
            {"Latency test failed; automatic routing is still available", "Не удалось проверить задержку; автоматическая маршрутизация по-прежнему доступна"},
            {"Server Location Test Failed", "Ошибка проверки серверов"}, {"Testing GeForce NOW server locations...", "Проверка серверов GeForce NOW..."}
        }},
        {"uk", {
            {"Server location", "Розташування сервера"}, {"Use automatic routing or choose a specific GeForce NOW data center.", "Використовуйте автоматичну маршрутизацію або виберіть центр обробки даних GeForce NOW."},
            {"Location", "Сервер"}, {"Locations are ranked by direct latency from this console.", "Сервери впорядковано за прямою затримкою з цієї консолі."},
            {"Latency test", "Тест затримки"}, {"Reload available locations and test each connection again.", "Повторно завантажити доступні сервери та перевірити кожне з'єднання."},
            {"Testing locations...", "Перевірка серверів..."}, {"Auto (best)", "Авто (найкращий)"}, {"Auto", "Авто"},
            {"Selected server", "Вибраний сервер"}, {"Current server (not advertised)", "Поточний сервер (не опублікований)"},
            {"Unavailable", "Недоступний"}, {"Best", "Найкращий"}, {"Server location test is already running", "Перевірка серверів уже виконується"},
            {"Server Locations Unavailable", "Сервери недоступні"}, {"Connect a GeForce NOW account before loading server locations.", "Підключіть обліковий запис GeForce NOW перед завантаженням серверів."},
            {"GeForce NOW did not return any server locations.", "GeForce NOW не повернув список серверів."}, {"Server location latency test complete", "Перевірку затримки серверів завершено"},
            {"Latency test failed; automatic routing is still available", "Не вдалося перевірити затримку; автоматична маршрутизація все ще доступна"},
            {"Server Location Test Failed", "Помилка перевірки серверів"}, {"Testing GeForce NOW server locations...", "Перевірка серверів GeForce NOW..."}
        }},
        {"es", {
            {"Server location", "Ubicación del servidor"}, {"Use automatic routing or choose a specific GeForce NOW data center.", "Usa el enrutamiento automático o elige un centro de datos de GeForce NOW."},
            {"Location", "Servidor"}, {"Locations are ranked by direct latency from this console.", "Los servidores se ordenan por latencia directa desde esta consola."},
            {"Latency test", "Prueba de latencia"}, {"Reload available locations and test each connection again.", "Vuelve a cargar los servidores disponibles y prueba cada conexión."},
            {"Testing locations...", "Probando servidores..."}, {"Auto (best)", "Auto (mejor)"}, {"Auto", "Auto"},
            {"Selected server", "Servidor seleccionado"}, {"Current server (not advertised)", "Servidor actual (no anunciado)"},
            {"Unavailable", "No disponible"}, {"Best", "Mejor"}, {"Server location test is already running", "La prueba de servidores ya está en curso"},
            {"Server Locations Unavailable", "Servidores no disponibles"}, {"Connect a GeForce NOW account before loading server locations.", "Conecta una cuenta de GeForce NOW antes de cargar los servidores."},
            {"GeForce NOW did not return any server locations.", "GeForce NOW no devolvió ningún servidor."}, {"Server location latency test complete", "Prueba de latencia completada"},
            {"Latency test failed; automatic routing is still available", "La prueba falló; el enrutamiento automático sigue disponible"},
            {"Server Location Test Failed", "Falló la prueba de servidores"}, {"Testing GeForce NOW server locations...", "Probando servidores de GeForce NOW..."}
        }},
        {"it", {
            {"Server location", "Posizione server"}, {"Use automatic routing or choose a specific GeForce NOW data center.", "Usa l'instradamento automatico o scegli un data center GeForce NOW."},
            {"Location", "Server"}, {"Locations are ranked by direct latency from this console.", "I server sono ordinati per latenza diretta da questa console."},
            {"Latency test", "Test latenza"}, {"Reload available locations and test each connection again.", "Ricarica i server disponibili e verifica ogni connessione."},
            {"Testing locations...", "Test dei server..."}, {"Auto (best)", "Auto (migliore)"}, {"Auto", "Auto"},
            {"Selected server", "Server selezionato"}, {"Current server (not advertised)", "Server attuale (non pubblicizzato)"},
            {"Unavailable", "Non disponibile"}, {"Best", "Migliore"}, {"Server location test is already running", "Il test dei server è già in corso"},
            {"Server Locations Unavailable", "Server non disponibili"}, {"Connect a GeForce NOW account before loading server locations.", "Collega un account GeForce NOW prima di caricare i server."},
            {"GeForce NOW did not return any server locations.", "GeForce NOW non ha restituito alcun server."}, {"Server location latency test complete", "Test della latenza completato"},
            {"Latency test failed; automatic routing is still available", "Test non riuscito; l'instradamento automatico è ancora disponibile"},
            {"Server Location Test Failed", "Test dei server non riuscito"}, {"Testing GeForce NOW server locations...", "Test dei server GeForce NOW..."}
        }},
        {"fr", {
            {"Server location", "Emplacement du serveur"}, {"Use automatic routing or choose a specific GeForce NOW data center.", "Utilisez le routage automatique ou choisissez un centre de données GeForce NOW."},
            {"Location", "Serveur"}, {"Locations are ranked by direct latency from this console.", "Les serveurs sont classés selon la latence directe depuis cette console."},
            {"Latency test", "Test de latence"}, {"Reload available locations and test each connection again.", "Rechargez les serveurs disponibles et testez chaque connexion."},
            {"Testing locations...", "Test des serveurs..."}, {"Auto (best)", "Auto (meilleur)"}, {"Auto", "Auto"},
            {"Selected server", "Serveur sélectionné"}, {"Current server (not advertised)", "Serveur actuel (non publié)"},
            {"Unavailable", "Indisponible"}, {"Best", "Meilleur"}, {"Server location test is already running", "Le test des serveurs est déjà en cours"},
            {"Server Locations Unavailable", "Serveurs indisponibles"}, {"Connect a GeForce NOW account before loading server locations.", "Connectez un compte GeForce NOW avant de charger les serveurs."},
            {"GeForce NOW did not return any server locations.", "GeForce NOW n'a renvoyé aucun serveur."}, {"Server location latency test complete", "Test de latence terminé"},
            {"Latency test failed; automatic routing is still available", "Le test a échoué; le routage automatique reste disponible"},
            {"Server Location Test Failed", "Échec du test des serveurs"}, {"Testing GeForce NOW server locations...", "Test des serveurs GeForce NOW..."}
        }},
        {"pl", {
            {"Server location", "Lokalizacja serwera"}, {"Use automatic routing or choose a specific GeForce NOW data center.", "Użyj automatycznego routingu lub wybierz centrum danych GeForce NOW."},
            {"Location", "Serwer"}, {"Locations are ranked by direct latency from this console.", "Serwery są sortowane według bezpośredniego opóźnienia z tej konsoli."},
            {"Latency test", "Test opóźnienia"}, {"Reload available locations and test each connection again.", "Załaduj ponownie dostępne serwery i sprawdź każde połączenie."},
            {"Testing locations...", "Testowanie serwerów..."}, {"Auto (best)", "Auto (najlepszy)"}, {"Auto", "Auto"},
            {"Selected server", "Wybrany serwer"}, {"Current server (not advertised)", "Bieżący serwer (niepublikowany)"},
            {"Unavailable", "Niedostępny"}, {"Best", "Najlepszy"}, {"Server location test is already running", "Test serwerów już trwa"},
            {"Server Locations Unavailable", "Serwery niedostępne"}, {"Connect a GeForce NOW account before loading server locations.", "Połącz konto GeForce NOW przed załadowaniem serwerów."},
            {"GeForce NOW did not return any server locations.", "GeForce NOW nie zwrócił listy serwerów."}, {"Server location latency test complete", "Test opóźnienia zakończony"},
            {"Latency test failed; automatic routing is still available", "Test nie powiódł się; routing automatyczny jest nadal dostępny"},
            {"Server Location Test Failed", "Test serwerów nie powiódł się"}, {"Testing GeForce NOW server locations...", "Testowanie serwerów GeForce NOW..."}
        }},
        {"zh-CN", {
            {"Server location", "服务器位置"}, {"Use automatic routing or choose a specific GeForce NOW data center.", "使用自动路由或选择特定的 GeForce NOW 数据中心。"},
            {"Location", "服务器"}, {"Locations are ranked by direct latency from this console.", "服务器按此主机的直连延迟排序。"},
            {"Latency test", "延迟测试"}, {"Reload available locations and test each connection again.", "重新加载可用服务器并再次测试每个连接。"},
            {"Testing locations...", "正在测试服务器..."}, {"Auto (best)", "自动（最佳）"}, {"Auto", "自动"},
            {"Selected server", "已选服务器"}, {"Current server (not advertised)", "当前服务器（未公开）"},
            {"Unavailable", "不可用"}, {"Best", "最佳"}, {"Server location test is already running", "服务器测试已在进行"},
            {"Server Locations Unavailable", "服务器不可用"}, {"Connect a GeForce NOW account before loading server locations.", "请先连接 GeForce NOW 账户再加载服务器。"},
            {"GeForce NOW did not return any server locations.", "GeForce NOW 未返回服务器列表。"}, {"Server location latency test complete", "服务器延迟测试完成"},
            {"Latency test failed; automatic routing is still available", "延迟测试失败；自动路由仍可使用"},
            {"Server Location Test Failed", "服务器测试失败"}, {"Testing GeForce NOW server locations...", "正在测试 GeForce NOW 服务器..."}
        }}
    };
    const auto found = dictionaries.find(code);
    return found == dictionaries.end() ? empty : found->second;
}

const Dictionary& CompletionDictionaryFor(const std::string& code)
{
    static const Dictionary empty;
    static const std::unordered_map<std::string, Dictionary> dictionaries = {
        {"ru", {
            {"Input", "Ввод"}, {"By", "Разработчик:"}, {"Unknown studio", "Неизвестная студия"},
            {"Unknown store", "Неизвестный магазин"}, {"About this game", "Об игре"},
            {"Up / Down  Scroll", "Вверх / вниз  Прокрутка"},
            {"Up / Down  Scroll  |  B  Back", "Вверх / вниз  Прокрутка  |  B  Назад"},
            {"No description is available yet for this title.", "Описание этой игры пока недоступно."},
            {"Browse screenshots", "Листать скриншоты"}, {"Publisher", "Издатель"},
            {"Last played", "Последний запуск"}, {"Unknown", "Неизвестно"}, {"Never", "Никогда"},
            {"library title", "игра библиотеки"}, {"catalog title", "игра каталога"},
            {"selected in library", "выбрано в библиотеке"},
            {"Previous screenshot", "Предыдущий скриншот"}, {"Next screenshot", "Следующий скриншот"},
            {"NTE Auto-login: Ready", "Автовход NTE: готов"},
            {"NTE Auto-login: Set email and password", "Автовход NTE: укажите почту и пароль"},
            {"Free To Play", "Бесплатная"}, {"Action", "Экшен"}, {"Adventure", "Приключения"},
            {"Role Playing", "Ролевая игра"}, {"Mouse", "Мышь"}, {"Keyboard", "Клавиатура"},
            {"Gamepad Partial", "Частичная поддержка геймпада"}, {"Gamepad", "Геймпад"},
            {"All stores", "Все магазины"}, {"Last Played", "Последний запуск"},
            {"More / Refresh", "Ещё / обновить"}, {"A-Z", "А-Я"},
            {"Loaded {0} games.", "Загружено игр: {0}."},
            {"Loaded {0} supported games.", "Загружено поддерживаемых игр: {0}."},
            {"Filter: {0}.", "Фильтр: {0}."}, {"Sort: {0}.", "Сортировка: {0}."},
            {"Found {0} matches.", "Найдено совпадений: {0}."},
            {"Showing first {0}. Press X or select Show more.", "Показаны первые {0}. Нажмите X или выберите «Показать ещё»."},
            {"Press X to refresh.", "Нажмите X для обновления."},
            {"Showing {0}-{1} of {2} (page {3}/{4}).", "Показано {0}-{1} из {2} (страница {3}/{4})."},
            {"Show more games ({0} left)", "Показать ещё ({0} осталось)"},
            {"Refresh catalog", "Обновить каталог"}, {"Position in queue:{0}", "Позиция в очереди:{0}"},
            {"GeForce NOW is allocating hardware for your game.", "GeForce NOW выделяет оборудование для вашей игры."},
            {"Native GeForce NOW client for Nintendo Switch.", "Нативный клиент GeForce NOW для Nintendo Switch."},
            {"Version", "Версия"}, {"Languages", "Языки"},
            {"Manage your GeForce NOW identity and persistent sign-in.", "Управление аккаунтом GeForce NOW и постоянным входом."},
            {"Balance image quality, latency and decoder stability.", "Баланс качества изображения, задержки и стабильности декодера."},
            {"Choose in-game localization and NVIDIA graphics persistence.", "Выбор языка игры и сохранения настроек графики NVIDIA."},
            {"Choose whether face buttons follow Xbox positions or Switch labels.", "Выбор раскладки лицевых кнопок Xbox или Switch."},
            {"Tune stream sound without changing the console volume.", "Настройка звука трансляции без изменения громкости консоли."},
            {"Inspect cache usage, diagnostics and local app data.", "Просмотр кэша, диагностики и локальных данных приложения."}
        }},
        {"uk", {
            {"Input", "Введення"}, {"By", "Розробник:"}, {"Unknown studio", "Невідома студія"},
            {"Unknown store", "Невідомий магазин"}, {"About this game", "Про гру"},
            {"Up / Down  Scroll", "Вгору / вниз  Прокрутка"},
            {"Up / Down  Scroll  |  B  Back", "Вгору / вниз  Прокрутка  |  B  Назад"},
            {"No description is available yet for this title.", "Опис цієї гри поки недоступний."},
            {"Browse screenshots", "Гортати знімки"}, {"Publisher", "Видавець"},
            {"Last played", "Останній запуск"}, {"Unknown", "Невідомо"}, {"Never", "Ніколи"},
            {"library title", "гра бібліотеки"}, {"catalog title", "гра каталогу"},
            {"selected in library", "вибрано в бібліотеці"},
            {"Previous screenshot", "Попередній знімок"}, {"Next screenshot", "Наступний знімок"},
            {"NTE Auto-login: Ready", "Автовхід NTE: готовий"},
            {"NTE Auto-login: Set email and password", "Автовхід NTE: вкажіть пошту й пароль"},
            {"Free To Play", "Безкоштовна"}, {"Action", "Екшен"}, {"Adventure", "Пригоди"},
            {"Role Playing", "Рольова гра"}, {"Mouse", "Миша"}, {"Keyboard", "Клавіатура"},
            {"Gamepad Partial", "Часткова підтримка геймпада"}, {"Gamepad", "Геймпад"},
            {"All stores", "Усі магазини"}, {"Last Played", "Останній запуск"},
            {"More / Refresh", "Ще / оновити"}, {"A-Z", "А-Я"},
            {"Loaded {0} games.", "Завантажено ігор: {0}."},
            {"Loaded {0} supported games.", "Завантажено підтримуваних ігор: {0}."},
            {"Filter: {0}.", "Фільтр: {0}."}, {"Sort: {0}.", "Сортування: {0}."},
            {"Found {0} matches.", "Знайдено збігів: {0}."},
            {"Showing first {0}. Press X or select Show more.", "Показано перші {0}. Натисніть X або виберіть «Показати ще»."},
            {"Press X to refresh.", "Натисніть X для оновлення."},
            {"Showing {0}-{1} of {2} (page {3}/{4}).", "Показано {0}-{1} із {2} (сторінка {3}/{4})."},
            {"Show more games ({0} left)", "Показати ще ({0} залишилось)"},
            {"Refresh catalog", "Оновити каталог"}, {"Position in queue:{0}", "Позиція в черзі:{0}"},
            {"GeForce NOW is allocating hardware for your game.", "GeForce NOW виділяє обладнання для вашої гри."},
            {"Native GeForce NOW client for Nintendo Switch.", "Нативний клієнт GeForce NOW для Nintendo Switch."},
            {"Version", "Версія"}, {"Languages", "Мови"},
            {"Manage your GeForce NOW identity and persistent sign-in.", "Керування обліковим записом GeForce NOW і постійним входом."},
            {"Balance image quality, latency and decoder stability.", "Баланс якості зображення, затримки та стабільності декодера."},
            {"Choose in-game localization and NVIDIA graphics persistence.", "Вибір мови гри та збереження налаштувань графіки NVIDIA."},
            {"Choose whether face buttons follow Xbox positions or Switch labels.", "Вибір розкладки лицьових кнопок Xbox або Switch."},
            {"Tune stream sound without changing the console volume.", "Налаштування звуку трансляції без зміни гучності консолі."},
            {"Inspect cache usage, diagnostics and local app data.", "Перегляд кешу, діагностики та локальних даних застосунку."}
        }},
        {"es", {
            {"Input", "Entrada"}, {"By", "Desarrollador:"}, {"Unknown studio", "Estudio desconocido"},
            {"Unknown store", "Tienda desconocida"}, {"About this game", "Acerca del juego"},
            {"Up / Down  Scroll", "Arriba / abajo  Desplazar"},
            {"Up / Down  Scroll  |  B  Back", "Arriba / abajo  Desplazar  |  B  Volver"},
            {"No description is available yet for this title.", "Todavía no hay descripción disponible para este juego."},
            {"Browse screenshots", "Ver capturas"}, {"Publisher", "Editor"},
            {"Last played", "Última partida"}, {"Unknown", "Desconocido"}, {"Never", "Nunca"},
            {"library title", "juego de la biblioteca"}, {"catalog title", "juego del catálogo"},
            {"selected in library", "seleccionado en la biblioteca"},
            {"Previous screenshot", "Captura anterior"}, {"Next screenshot", "Captura siguiente"},
            {"NTE Auto-login: Ready", "Inicio automático NTE: listo"},
            {"NTE Auto-login: Set email and password", "Inicio automático NTE: introduce correo y contraseña"},
            {"Free To Play", "Gratis"}, {"Action", "Acción"}, {"Adventure", "Aventura"},
            {"Role Playing", "Rol"}, {"Mouse", "Ratón"}, {"Keyboard", "Teclado"},
            {"Gamepad Partial", "Mando parcial"}, {"Gamepad", "Mando"},
            {"All stores", "Todas las tiendas"}, {"Last Played", "Última partida"},
            {"More / Refresh", "Más / actualizar"}, {"A-Z", "A-Z"},
            {"Loaded {0} games.", "{0} juegos cargados."},
            {"Loaded {0} supported games.", "{0} juegos compatibles cargados."},
            {"Filter: {0}.", "Filtro: {0}."}, {"Sort: {0}.", "Orden: {0}."},
            {"Found {0} matches.", "{0} resultados encontrados."},
            {"Showing first {0}. Press X or select Show more.", "Se muestran los primeros {0}. Pulsa X o elige Mostrar más."},
            {"Press X to refresh.", "Pulsa X para actualizar."},
            {"Showing {0}-{1} of {2} (page {3}/{4}).", "Mostrando {0}-{1} de {2} (página {3}/{4})."},
            {"Show more games ({0} left)", "Mostrar más ({0} restantes)"},
            {"Refresh catalog", "Actualizar catálogo"}, {"Position in queue:{0}", "Posición en la cola:{0}"},
            {"GeForce NOW is allocating hardware for your game.", "GeForce NOW está asignando hardware para tu juego."},
            {"Native GeForce NOW client for Nintendo Switch.", "Cliente nativo de GeForce NOW para Nintendo Switch."},
            {"Version", "Versión"}, {"Languages", "Idiomas"},
            {"Manage your GeForce NOW identity and persistent sign-in.", "Gestiona tu identidad de GeForce NOW y el inicio de sesión persistente."},
            {"Balance image quality, latency and decoder stability.", "Equilibra la calidad de imagen, la latencia y la estabilidad del decodificador."},
            {"Choose in-game localization and NVIDIA graphics persistence.", "Elige el idioma del juego y la persistencia de gráficos de NVIDIA."},
            {"Choose whether face buttons follow Xbox positions or Switch labels.", "Elige la disposición de botones de Xbox o las etiquetas de Switch."},
            {"Tune stream sound without changing the console volume.", "Ajusta el audio de la transmisión sin cambiar el volumen de la consola."},
            {"Inspect cache usage, diagnostics and local app data.", "Consulta la caché, los diagnósticos y los datos locales."}
        }},
        {"it", {
            {"Input", "Input"}, {"By", "Sviluppatore:"}, {"Unknown studio", "Studio sconosciuto"},
            {"Unknown store", "Negozio sconosciuto"}, {"About this game", "Informazioni sul gioco"},
            {"Up / Down  Scroll", "Su / giù  Scorri"},
            {"Up / Down  Scroll  |  B  Back", "Su / giù  Scorri  |  B  Indietro"},
            {"No description is available yet for this title.", "La descrizione di questo gioco non è ancora disponibile."},
            {"Browse screenshots", "Sfoglia schermate"}, {"Publisher", "Editore"},
            {"Last played", "Ultima partita"}, {"Unknown", "Sconosciuto"}, {"Never", "Mai"},
            {"library title", "gioco della libreria"}, {"catalog title", "gioco del catalogo"},
            {"selected in library", "selezionato nella libreria"},
            {"Previous screenshot", "Schermata precedente"}, {"Next screenshot", "Schermata successiva"},
            {"NTE Auto-login: Ready", "Accesso automatico NTE: pronto"},
            {"NTE Auto-login: Set email and password", "Accesso automatico NTE: inserisci email e password"},
            {"Free To Play", "Gratuito"}, {"Action", "Azione"}, {"Adventure", "Avventura"},
            {"Role Playing", "Gioco di ruolo"}, {"Mouse", "Mouse"}, {"Keyboard", "Tastiera"},
            {"Gamepad Partial", "Gamepad parziale"}, {"Gamepad", "Gamepad"},
            {"All stores", "Tutti i negozi"}, {"Last Played", "Ultima partita"},
            {"More / Refresh", "Altro / aggiorna"}, {"A-Z", "A-Z"},
            {"Loaded {0} games.", "{0} giochi caricati."},
            {"Loaded {0} supported games.", "{0} giochi supportati caricati."},
            {"Filter: {0}.", "Filtro: {0}."}, {"Sort: {0}.", "Ordine: {0}."},
            {"Found {0} matches.", "{0} risultati trovati."},
            {"Showing first {0}. Press X or select Show more.", "Visualizzati i primi {0}. Premi X o scegli Mostra altro."},
            {"Press X to refresh.", "Premi X per aggiornare."},
            {"Showing {0}-{1} of {2} (page {3}/{4}).", "Visualizzati {0}-{1} di {2} (pagina {3}/{4})."},
            {"Show more games ({0} left)", "Mostra altro ({0} rimanenti)"},
            {"Refresh catalog", "Aggiorna catalogo"}, {"Position in queue:{0}", "Posizione in coda:{0}"},
            {"GeForce NOW is allocating hardware for your game.", "GeForce NOW sta assegnando l'hardware per il gioco."},
            {"Native GeForce NOW client for Nintendo Switch.", "Client GeForce NOW nativo per Nintendo Switch."},
            {"Version", "Versione"}, {"Languages", "Lingue"},
            {"Manage your GeForce NOW identity and persistent sign-in.", "Gestisci l'identità GeForce NOW e l'accesso persistente."},
            {"Balance image quality, latency and decoder stability.", "Bilancia qualità dell'immagine, latenza e stabilità del decoder."},
            {"Choose in-game localization and NVIDIA graphics persistence.", "Scegli la lingua di gioco e la persistenza grafica NVIDIA."},
            {"Choose whether face buttons follow Xbox positions or Switch labels.", "Scegli la disposizione dei tasti Xbox o le etichette Switch."},
            {"Tune stream sound without changing the console volume.", "Regola l'audio dello streaming senza cambiare il volume della console."},
            {"Inspect cache usage, diagnostics and local app data.", "Controlla cache, diagnostica e dati locali dell'app."}
        }},
        {"fr", {
            {"Input", "Entrée"}, {"By", "Développeur :"}, {"Unknown studio", "Studio inconnu"},
            {"Unknown store", "Boutique inconnue"}, {"About this game", "À propos du jeu"},
            {"Up / Down  Scroll", "Haut / bas  Défiler"},
            {"Up / Down  Scroll  |  B  Back", "Haut / bas  Défiler  |  B  Retour"},
            {"No description is available yet for this title.", "Aucune description n'est encore disponible pour ce jeu."},
            {"Browse screenshots", "Parcourir les captures"}, {"Publisher", "Éditeur"},
            {"Last played", "Dernière partie"}, {"Unknown", "Inconnu"}, {"Never", "Jamais"},
            {"library title", "jeu de la bibliothèque"}, {"catalog title", "jeu du catalogue"},
            {"selected in library", "sélectionné dans la bibliothèque"},
            {"Previous screenshot", "Capture précédente"}, {"Next screenshot", "Capture suivante"},
            {"NTE Auto-login: Ready", "Connexion auto NTE : prête"},
            {"NTE Auto-login: Set email and password", "Connexion auto NTE : saisissez l'e-mail et le mot de passe"},
            {"Free To Play", "Gratuit"}, {"Action", "Action"}, {"Adventure", "Aventure"},
            {"Role Playing", "Jeu de rôle"}, {"Mouse", "Souris"}, {"Keyboard", "Clavier"},
            {"Gamepad Partial", "Manette partielle"}, {"Gamepad", "Manette"},
            {"All stores", "Toutes les boutiques"}, {"Last Played", "Dernière partie"},
            {"More / Refresh", "Plus / actualiser"}, {"A-Z", "A-Z"},
            {"Loaded {0} games.", "{0} jeux chargés."},
            {"Loaded {0} supported games.", "{0} jeux compatibles chargés."},
            {"Filter: {0}.", "Filtre : {0}."}, {"Sort: {0}.", "Tri : {0}."},
            {"Found {0} matches.", "{0} résultats trouvés."},
            {"Showing first {0}. Press X or select Show more.", "Affichage des {0} premiers. Appuyez sur X ou choisissez Afficher plus."},
            {"Press X to refresh.", "Appuyez sur X pour actualiser."},
            {"Showing {0}-{1} of {2} (page {3}/{4}).", "Affichage de {0}-{1} sur {2} (page {3}/{4})."},
            {"Show more games ({0} left)", "Afficher plus ({0} restants)"},
            {"Refresh catalog", "Actualiser le catalogue"}, {"Position in queue:{0}", "Position dans la file :{0}"},
            {"GeForce NOW is allocating hardware for your game.", "GeForce NOW attribue du matériel à votre jeu."},
            {"Native GeForce NOW client for Nintendo Switch.", "Client GeForce NOW natif pour Nintendo Switch."},
            {"Version", "Version"}, {"Languages", "Langues"},
            {"Manage your GeForce NOW identity and persistent sign-in.", "Gérez votre identité GeForce NOW et la connexion persistante."},
            {"Balance image quality, latency and decoder stability.", "Équilibrez la qualité d'image, la latence et la stabilité du décodeur."},
            {"Choose in-game localization and NVIDIA graphics persistence.", "Choisissez la langue du jeu et la persistance graphique NVIDIA."},
            {"Choose whether face buttons follow Xbox positions or Switch labels.", "Choisissez la disposition Xbox ou les libellés Switch."},
            {"Tune stream sound without changing the console volume.", "Réglez le son du flux sans modifier le volume de la console."},
            {"Inspect cache usage, diagnostics and local app data.", "Consultez le cache, les diagnostics et les données locales."}
        }},
        {"pl", {
            {"Input", "Sterowanie"}, {"By", "Twórca:"}, {"Unknown studio", "Nieznane studio"},
            {"Unknown store", "Nieznany sklep"}, {"About this game", "O grze"},
            {"Up / Down  Scroll", "Góra / dół  Przewijanie"},
            {"Up / Down  Scroll  |  B  Back", "Góra / dół  Przewijanie  |  B  Wstecz"},
            {"No description is available yet for this title.", "Opis tej gry nie jest jeszcze dostępny."},
            {"Browse screenshots", "Przeglądaj zrzuty"}, {"Publisher", "Wydawca"},
            {"Last played", "Ostatnia gra"}, {"Unknown", "Nieznane"}, {"Never", "Nigdy"},
            {"library title", "gra z biblioteki"}, {"catalog title", "gra z katalogu"},
            {"selected in library", "wybrano w bibliotece"},
            {"Previous screenshot", "Poprzedni zrzut"}, {"Next screenshot", "Następny zrzut"},
            {"NTE Auto-login: Ready", "Autologowanie NTE: gotowe"},
            {"NTE Auto-login: Set email and password", "Autologowanie NTE: podaj e-mail i hasło"},
            {"Free To Play", "Darmowa"}, {"Action", "Akcja"}, {"Adventure", "Przygodowa"},
            {"Role Playing", "RPG"}, {"Mouse", "Mysz"}, {"Keyboard", "Klawiatura"},
            {"Gamepad Partial", "Częściowa obsługa pada"}, {"Gamepad", "Gamepad"},
            {"All stores", "Wszystkie sklepy"}, {"Last Played", "Ostatnia gra"},
            {"More / Refresh", "Więcej / odśwież"}, {"A-Z", "A-Z"},
            {"Loaded {0} games.", "Wczytano gier: {0}."},
            {"Loaded {0} supported games.", "Wczytano obsługiwanych gier: {0}."},
            {"Filter: {0}.", "Filtr: {0}."}, {"Sort: {0}.", "Sortowanie: {0}."},
            {"Found {0} matches.", "Znaleziono: {0}."},
            {"Showing first {0}. Press X or select Show more.", "Wyświetlono pierwsze {0}. Naciśnij X lub wybierz Pokaż więcej."},
            {"Press X to refresh.", "Naciśnij X, aby odświeżyć."},
            {"Showing {0}-{1} of {2} (page {3}/{4}).", "Wyświetlono {0}-{1} z {2} (strona {3}/{4})."},
            {"Show more games ({0} left)", "Pokaż więcej (pozostało {0})"},
            {"Refresh catalog", "Odśwież katalog"}, {"Position in queue:{0}", "Pozycja w kolejce:{0}"},
            {"GeForce NOW is allocating hardware for your game.", "GeForce NOW przydziela sprzęt do gry."},
            {"Native GeForce NOW client for Nintendo Switch.", "Natywny klient GeForce NOW dla Nintendo Switch."},
            {"Version", "Wersja"}, {"Languages", "Języki"},
            {"Manage your GeForce NOW identity and persistent sign-in.", "Zarządzaj kontem GeForce NOW i trwałym logowaniem."},
            {"Balance image quality, latency and decoder stability.", "Zrównoważ jakość obrazu, opóźnienie i stabilność dekodera."},
            {"Choose in-game localization and NVIDIA graphics persistence.", "Wybierz język gry i zachowywanie grafiki NVIDIA."},
            {"Choose whether face buttons follow Xbox positions or Switch labels.", "Wybierz układ przycisków Xbox lub oznaczenia Switch."},
            {"Tune stream sound without changing the console volume.", "Dostosuj dźwięk transmisji bez zmiany głośności konsoli."},
            {"Inspect cache usage, diagnostics and local app data.", "Sprawdź pamięć podręczną, diagnostykę i dane lokalne."}
        }},
        {"zh-CN", {
            {"Input", "输入"}, {"By", "开发者："}, {"Unknown studio", "未知工作室"},
            {"Unknown store", "未知商店"}, {"About this game", "关于此游戏"},
            {"Up / Down  Scroll", "上 / 下  滚动"},
            {"Up / Down  Scroll  |  B  Back", "上 / 下  滚动  |  B  返回"},
            {"No description is available yet for this title.", "此游戏暂无说明。"},
            {"Browse screenshots", "浏览截图"}, {"Publisher", "发行商"},
            {"Last played", "上次游玩"}, {"Unknown", "未知"}, {"Never", "从未"},
            {"library title", "库中游戏"}, {"catalog title", "目录游戏"},
            {"selected in library", "已在游戏库中选择"},
            {"Previous screenshot", "上一张截图"}, {"Next screenshot", "下一张截图"},
            {"NTE Auto-login: Ready", "NTE 自动登录：已就绪"},
            {"NTE Auto-login: Set email and password", "NTE 自动登录：设置邮箱和密码"},
            {"Free To Play", "免费游玩"}, {"Action", "动作"}, {"Adventure", "冒险"},
            {"Role Playing", "角色扮演"}, {"Mouse", "鼠标"}, {"Keyboard", "键盘"},
            {"Gamepad Partial", "部分手柄支持"}, {"Gamepad", "手柄"},
            {"All stores", "所有商店"}, {"Last Played", "上次游玩"},
            {"More / Refresh", "更多 / 刷新"}, {"A-Z", "A-Z"},
            {"Loaded {0} games.", "已加载 {0} 款游戏。"},
            {"Loaded {0} supported games.", "已加载 {0} 款支持的游戏。"},
            {"Filter: {0}.", "筛选：{0}。"}, {"Sort: {0}.", "排序：{0}。"},
            {"Found {0} matches.", "找到 {0} 个结果。"},
            {"Showing first {0}. Press X or select Show more.", "显示前 {0} 项。按 X 或选择显示更多。"},
            {"Press X to refresh.", "按 X 刷新。"},
            {"Showing {0}-{1} of {2} (page {3}/{4}).", "显示 {0}-{1} / {2}（第 {3}/{4} 页）。"},
            {"Show more games ({0} left)", "显示更多（剩余 {0}）"},
            {"Refresh catalog", "刷新目录"}, {"Position in queue:{0}", "队列位置：{0}"},
            {"GeForce NOW is allocating hardware for your game.", "GeForce NOW 正在为游戏分配硬件。"},
            {"Native GeForce NOW client for Nintendo Switch.", "Nintendo Switch 原生 GeForce NOW 客户端。"},
            {"Version", "版本"}, {"Languages", "语言"},
            {"Manage your GeForce NOW identity and persistent sign-in.", "管理 GeForce NOW 身份和持久登录。"},
            {"Balance image quality, latency and decoder stability.", "平衡画质、延迟和解码器稳定性。"},
            {"Choose in-game localization and NVIDIA graphics persistence.", "选择游戏语言和 NVIDIA 图形设置保存。"},
            {"Choose whether face buttons follow Xbox positions or Switch labels.", "选择 Xbox 按键位置或 Switch 按键标签。"},
            {"Tune stream sound without changing the console volume.", "调整串流声音而不改变主机音量。"},
            {"Inspect cache usage, diagnostics and local app data.", "查看缓存、诊断和本地应用数据。"}
        }}
    };
    const auto found = dictionaries.find(code);
    return found == dictionaries.end() ? empty : found->second;
}

const Dictionary& SearchAndLaunchDictionaryFor(const std::string& code)
{
    static const Dictionary empty;
    static const std::unordered_map<std::string, Dictionary> dictionaries = {
        {"ru", {
            {"All", "\u0412\u0441\u0435"},
            {"Clear search", "\u041e\u0447\u0438\u0441\u0442\u0438\u0442\u044c \u043f\u043e\u0438\u0441\u043a"},
            {"Search Library", "\u041f\u043e\u0438\u0441\u043a \u0432 \u0431\u0438\u0431\u043b\u0438\u043e\u0442\u0435\u043a\u0435"},
            {"Enter a title to filter your games", "\u0412\u0432\u0435\u0434\u0438\u0442\u0435 \u043d\u0430\u0437\u0432\u0430\u043d\u0438\u0435 \u0438\u0433\u0440\u044b"},
            {"Search Catalog", "\u041f\u043e\u0438\u0441\u043a \u0432 \u043a\u0430\u0442\u0430\u043b\u043e\u0433\u0435"},
            {"Enter a title to filter games", "\u0412\u0432\u0435\u0434\u0438\u0442\u0435 \u043d\u0430\u0437\u0432\u0430\u043d\u0438\u0435 \u0438\u0433\u0440\u044b"},
            {"Searching for: {0}", "\u041f\u043e\u0438\u0441\u043a: {0}"},
            {"B  Clear search", "B  \u041e\u0447\u0438\u0441\u0442\u0438\u0442\u044c \u043f\u043e\u0438\u0441\u043a"},
            {"QUEUE", "\u041e\u0427\u0415\u0420\u0415\u0414\u042c"},
            {"SETUP", "\u041d\u0410\u0421\u0422\u0420\u041e\u0419\u041a\u0410"},
            {"READY", "\u0413\u041e\u0422\u041e\u0412\u041e"},
        }},
        {"uk", {
            {"All", "\u0423\u0441\u0456"},
            {"Clear search", "\u041e\u0447\u0438\u0441\u0442\u0438\u0442\u0438 \u043f\u043e\u0448\u0443\u043a"},
            {"Search Library", "\u041f\u043e\u0448\u0443\u043a \u0443 \u0431\u0456\u0431\u043b\u0456\u043e\u0442\u0435\u0446\u0456"},
            {"Enter a title to filter your games", "\u0412\u0432\u0435\u0434\u0456\u0442\u044c \u043d\u0430\u0437\u0432\u0443 \u0433\u0440\u0438"},
            {"Search Catalog", "\u041f\u043e\u0448\u0443\u043a \u0443 \u043a\u0430\u0442\u0430\u043b\u043e\u0437\u0456"},
            {"Enter a title to filter games", "\u0412\u0432\u0435\u0434\u0456\u0442\u044c \u043d\u0430\u0437\u0432\u0443 \u0433\u0440\u0438"},
            {"Searching for: {0}", "\u041f\u043e\u0448\u0443\u043a: {0}"},
            {"B  Clear search", "B  \u041e\u0447\u0438\u0441\u0442\u0438\u0442\u0438 \u043f\u043e\u0448\u0443\u043a"},
            {"QUEUE", "\u0427\u0415\u0420\u0413\u0410"},
            {"SETUP", "\u041d\u0410\u041b\u0410\u0428\u0422\u0423\u0412\u0410\u041d\u041d\u042f"},
            {"READY", "\u0413\u041e\u0422\u041e\u0412\u041e"},
        }},
        {"es", {
            {"All", "Todos"}, {"Clear search", "Borrar b\u00fasqueda"},
            {"Search Library", "Buscar en la biblioteca"},
            {"Enter a title to filter your games", "Introduce el t\u00edtulo de un juego"},
            {"Search Catalog", "Buscar en el cat\u00e1logo"},
            {"Enter a title to filter games", "Introduce el t\u00edtulo de un juego"},
            {"Searching for: {0}", "Buscando: {0}"},
            {"B  Clear search", "B  Borrar b\u00fasqueda"},
            {"QUEUE", "COLA"}, {"SETUP", "CONFIGURACI\u00d3N"}, {"READY", "LISTO"},
        }},
        {"it", {
            {"All", "Tutti"}, {"Clear search", "Cancella ricerca"},
            {"Search Library", "Cerca nella libreria"},
            {"Enter a title to filter your games", "Inserisci il titolo di un gioco"},
            {"Search Catalog", "Cerca nel catalogo"},
            {"Enter a title to filter games", "Inserisci il titolo di un gioco"},
            {"Searching for: {0}", "Ricerca: {0}"},
            {"B  Clear search", "B  Cancella ricerca"},
            {"QUEUE", "CODA"}, {"SETUP", "CONFIGURAZIONE"}, {"READY", "PRONTO"},
        }},
        {"fr", {
            {"All", "Tous"}, {"Clear search", "Effacer la recherche"},
            {"Search Library", "Rechercher dans la biblioth\u00e8que"},
            {"Enter a title to filter your games", "Saisissez le titre d'un jeu"},
            {"Search Catalog", "Rechercher dans le catalogue"},
            {"Enter a title to filter games", "Saisissez le titre d'un jeu"},
            {"Searching for: {0}", "Recherche : {0}"},
            {"B  Clear search", "B  Effacer la recherche"},
            {"QUEUE", "FILE"}, {"SETUP", "CONFIGURATION"}, {"READY", "PR\u00caT"},
        }},
        {"pl", {
            {"All", "Wszystkie"}, {"Clear search", "Wyczy\u015b\u0107 wyszukiwanie"},
            {"Search Library", "Przeszukaj bibliotek\u0119"},
            {"Enter a title to filter your games", "Wpisz tytu\u0142 gry"},
            {"Search Catalog", "Przeszukaj katalog"},
            {"Enter a title to filter games", "Wpisz tytu\u0142 gry"},
            {"Searching for: {0}", "Wyszukiwanie: {0}"},
            {"B  Clear search", "B  Wyczy\u015b\u0107 wyszukiwanie"},
            {"QUEUE", "KOLEJKA"}, {"SETUP", "KONFIGURACJA"}, {"READY", "GOTOWE"},
        }},
        {"zh-CN", {
            {"All", "\u5168\u90e8"},
            {"Clear search", "\u6e05\u9664\u641c\u7d22"},
            {"Search Library", "\u641c\u7d22\u6e38\u620f\u5e93"},
            {"Enter a title to filter your games", "\u8f93\u5165\u6e38\u620f\u540d\u79f0"},
            {"Search Catalog", "\u641c\u7d22\u76ee\u5f55"},
            {"Enter a title to filter games", "\u8f93\u5165\u6e38\u620f\u540d\u79f0"},
            {"Searching for: {0}", "\u6b63\u5728\u641c\u7d22\uff1a{0}"},
            {"B  Clear search", "B  \u6e05\u9664\u641c\u7d22"},
            {"QUEUE", "\u6392\u961f"}, {"SETUP", "\u8bbe\u7f6e"}, {"READY", "\u5c31\u7eea"},
        }},
    };
    const auto found = dictionaries.find(code);
    return found == dictionaries.end() ? empty : found->second;
}

const Dictionary& SessionLaunchDictionaryFor(const std::string& code)
{
    static const Dictionary empty;
    static const std::unordered_map<std::string, Dictionary> dictionaries = {
        {"ru", {
            {"Advertisement required", "Требуется реклама"},
            {"Waiting for NVIDIA to provide the Free-tier advertisement details.", "Ожидание данных рекламы для бесплатного тарифа от NVIDIA."},
            {"Free-tier advertisement ready", "Реклама для бесплатного тарифа готова"},
            {" ({0} seconds)", " ({0} с)"},
            {". Prepared format: {0}", ". Формат: {0}"},
            {". Playback will begin automatically.", ". Воспроизведение начнется автоматически."},
            {"Advertisement complete", "Реклама завершена"},
            {"Returning to cloud rig preparation.", "Возвращение к подготовке облачной системы."},
            {"NVIDIA is updating this game...\nThe session will start automatically when patching finishes.", "NVIDIA обновляет игру...\nСеанс начнется автоматически после завершения обновления."},
            {"In queue... Position: {0}", "В очереди... Позиция: {0}"},
            {"Setting up rig... Position: {0}", "Настройка облачной системы... Позиция: {0}"},
            {"Setting up rig... Please wait.", "Настройка облачной системы... Подождите."},
            {"Waiting for NVIDIA session ads or confirmation...", "Ожидание рекламы сеанса или подтверждения NVIDIA..."},
            {"Waiting... Status: {0}", "Ожидание... Статус: {0}"},
            {"Updating the game", "Обновление игры"},
            {"Waiting for an available cloud rig", "Ожидание доступной облачной системы"},
            {"Your cloud rig will start automatically when it is ready.", "Облачная система запустится автоматически, когда будет готова."},
            {"NVIDIA is patching this title. Waiting before retry {0} of 2.", "NVIDIA обновляет игру. Ожидание перед повтором {0} из 2."},
            {"Connecting to the streaming server", "Подключение к серверу трансляции"},
            {"The rig is ready. Configuring the secure network path.", "Облачная система готова. Настройка защищенного сетевого подключения."},
            {"Starting the video stream", "Запуск видеопотока"},
            {"Negotiating WebRTC and waiting for the first clean frame.", "Настройка WebRTC и ожидание первого целого кадра."},
        }},
        {"uk", {
            {"Advertisement required", "Потрібна реклама"},
            {"Waiting for NVIDIA to provide the Free-tier advertisement details.", "Очікування даних реклами для безкоштовного тарифу від NVIDIA."},
            {"Free-tier advertisement ready", "Реклама для безкоштовного тарифу готова"},
            {" ({0} seconds)", " ({0} с)"},
            {". Prepared format: {0}", ". Формат: {0}"},
            {". Playback will begin automatically.", ". Відтворення почнеться автоматично."},
            {"Advertisement complete", "Рекламу завершено"},
            {"Returning to cloud rig preparation.", "Повернення до підготовки хмарної системи."},
            {"NVIDIA is updating this game...\nThe session will start automatically when patching finishes.", "NVIDIA оновлює гру...\nСеанс почнеться автоматично після завершення оновлення."},
            {"In queue... Position: {0}", "У черзі... Позиція: {0}"},
            {"Setting up rig... Position: {0}", "Налаштування хмарної системи... Позиція: {0}"},
            {"Setting up rig... Please wait.", "Налаштування хмарної системи... Зачекайте."},
            {"Waiting for NVIDIA session ads or confirmation...", "Очікування реклами сеансу або підтвердження NVIDIA..."},
            {"Waiting... Status: {0}", "Очікування... Статус: {0}"},
            {"Updating the game", "Оновлення гри"},
            {"Waiting for an available cloud rig", "Очікування доступної хмарної системи"},
            {"Your cloud rig will start automatically when it is ready.", "Хмарна система запуститься автоматично, коли буде готова."},
            {"NVIDIA is patching this title. Waiting before retry {0} of 2.", "NVIDIA оновлює гру. Очікування перед повтором {0} із 2."},
            {"Connecting to the streaming server", "Підключення до сервера трансляції"},
            {"The rig is ready. Configuring the secure network path.", "Хмарна система готова. Налаштування захищеного мережевого з'єднання."},
            {"Starting the video stream", "Запуск відеопотоку"},
            {"Negotiating WebRTC and waiting for the first clean frame.", "Налаштування WebRTC та очікування першого цілого кадру."},
        }},
        {"es", {
            {"Advertisement required", "Se requiere publicidad"},
            {"Waiting for NVIDIA to provide the Free-tier advertisement details.", "Esperando los datos del anuncio del plan gratuito de NVIDIA."},
            {"Free-tier advertisement ready", "Anuncio del plan gratuito listo"},
            {" ({0} seconds)", " ({0} s)"}, {". Prepared format: {0}", ". Formato: {0}"},
            {". Playback will begin automatically.", ". La reproducción comenzará automáticamente."},
            {"Advertisement complete", "Anuncio finalizado"}, {"Returning to cloud rig preparation.", "Volviendo a preparar el equipo en la nube."},
            {"NVIDIA is updating this game...\nThe session will start automatically when patching finishes.", "NVIDIA está actualizando el juego...\nLa sesión comenzará automáticamente al terminar."},
            {"In queue... Position: {0}", "En cola... Posición: {0}"}, {"Setting up rig... Position: {0}", "Preparando el equipo... Posición: {0}"},
            {"Setting up rig... Please wait.", "Preparando el equipo... Espera."}, {"Waiting for NVIDIA session ads or confirmation...", "Esperando anuncios o confirmación de NVIDIA..."},
            {"Waiting... Status: {0}", "Esperando... Estado: {0}"}, {"Updating the game", "Actualizando el juego"},
            {"Waiting for an available cloud rig", "Esperando un equipo en la nube disponible"}, {"Your cloud rig will start automatically when it is ready.", "El equipo en la nube se iniciará automáticamente cuando esté listo."},
            {"NVIDIA is patching this title. Waiting before retry {0} of 2.", "NVIDIA está actualizando el juego. Esperando antes del reintento {0} de 2."},
            {"Connecting to the streaming server", "Conectando al servidor de streaming"}, {"The rig is ready. Configuring the secure network path.", "El equipo está listo. Configurando la conexión segura."},
            {"Starting the video stream", "Iniciando el vídeo"}, {"Negotiating WebRTC and waiting for the first clean frame.", "Configurando WebRTC y esperando el primer fotograma válido."},
        }},
        {"it", {
            {"Advertisement required", "Pubblicità richiesta"}, {"Waiting for NVIDIA to provide the Free-tier advertisement details.", "In attesa dei dati pubblicitari del piano gratuito da NVIDIA."},
            {"Free-tier advertisement ready", "Pubblicità del piano gratuito pronta"}, {" ({0} seconds)", " ({0} s)"}, {". Prepared format: {0}", ". Formato: {0}"},
            {". Playback will begin automatically.", ". La riproduzione inizierà automaticamente."}, {"Advertisement complete", "Pubblicità completata"},
            {"Returning to cloud rig preparation.", "Ritorno alla preparazione del PC cloud."}, {"NVIDIA is updating this game...\nThe session will start automatically when patching finishes.", "NVIDIA sta aggiornando il gioco...\nLa sessione inizierà automaticamente al termine."},
            {"In queue... Position: {0}", "In coda... Posizione: {0}"}, {"Setting up rig... Position: {0}", "Preparazione del PC... Posizione: {0}"},
            {"Setting up rig... Please wait.", "Preparazione del PC... Attendi."}, {"Waiting for NVIDIA session ads or confirmation...", "In attesa di pubblicità o conferma NVIDIA..."},
            {"Waiting... Status: {0}", "Attesa... Stato: {0}"}, {"Updating the game", "Aggiornamento del gioco"},
            {"Waiting for an available cloud rig", "In attesa di un PC cloud disponibile"}, {"Your cloud rig will start automatically when it is ready.", "Il PC cloud si avvierà automaticamente quando sarà pronto."},
            {"NVIDIA is patching this title. Waiting before retry {0} of 2.", "NVIDIA sta aggiornando il gioco. Attesa prima del tentativo {0} di 2."},
            {"Connecting to the streaming server", "Connessione al server di streaming"}, {"The rig is ready. Configuring the secure network path.", "Il PC è pronto. Configurazione della connessione sicura."},
            {"Starting the video stream", "Avvio del video"}, {"Negotiating WebRTC and waiting for the first clean frame.", "Configurazione di WebRTC e attesa del primo fotogramma valido."},
        }},
        {"fr", {
            {"Advertisement required", "Publicité requise"}, {"Waiting for NVIDIA to provide the Free-tier advertisement details.", "En attente des données publicitaires de l'offre gratuite de NVIDIA."},
            {"Free-tier advertisement ready", "Publicité de l'offre gratuite prête"}, {" ({0} seconds)", " ({0} s)"}, {". Prepared format: {0}", ". Format : {0}"},
            {". Playback will begin automatically.", ". La lecture commencera automatiquement."}, {"Advertisement complete", "Publicité terminée"},
            {"Returning to cloud rig preparation.", "Retour à la préparation de la machine cloud."}, {"NVIDIA is updating this game...\nThe session will start automatically when patching finishes.", "NVIDIA met le jeu à jour...\nLa session démarrera automatiquement à la fin."},
            {"In queue... Position: {0}", "Dans la file... Position : {0}"}, {"Setting up rig... Position: {0}", "Préparation de la machine... Position : {0}"},
            {"Setting up rig... Please wait.", "Préparation de la machine... Patientez."}, {"Waiting for NVIDIA session ads or confirmation...", "En attente des publicités ou de la confirmation NVIDIA..."},
            {"Waiting... Status: {0}", "En attente... État : {0}"}, {"Updating the game", "Mise à jour du jeu"},
            {"Waiting for an available cloud rig", "En attente d'une machine cloud disponible"}, {"Your cloud rig will start automatically when it is ready.", "La machine cloud démarrera automatiquement lorsqu'elle sera prête."},
            {"NVIDIA is patching this title. Waiting before retry {0} of 2.", "NVIDIA met le jeu à jour. Attente avant la tentative {0} sur 2."},
            {"Connecting to the streaming server", "Connexion au serveur de streaming"}, {"The rig is ready. Configuring the secure network path.", "La machine est prête. Configuration de la connexion sécurisée."},
            {"Starting the video stream", "Démarrage de la vidéo"}, {"Negotiating WebRTC and waiting for the first clean frame.", "Configuration de WebRTC et attente de la première image valide."},
        }},
        {"pl", {
            {"Advertisement required", "Wymagana reklama"}, {"Waiting for NVIDIA to provide the Free-tier advertisement details.", "Oczekiwanie na dane reklamy bezpłatnego planu od NVIDIA."},
            {"Free-tier advertisement ready", "Reklama bezpłatnego planu jest gotowa"}, {" ({0} seconds)", " ({0} s)"}, {". Prepared format: {0}", ". Format: {0}"},
            {". Playback will begin automatically.", ". Odtwarzanie rozpocznie się automatycznie."}, {"Advertisement complete", "Reklama zakończona"},
            {"Returning to cloud rig preparation.", "Powrót do przygotowania komputera w chmurze."}, {"NVIDIA is updating this game...\nThe session will start automatically when patching finishes.", "NVIDIA aktualizuje grę...\nSesja rozpocznie się automatycznie po zakończeniu."},
            {"In queue... Position: {0}", "W kolejce... Pozycja: {0}"}, {"Setting up rig... Position: {0}", "Przygotowywanie komputera... Pozycja: {0}"},
            {"Setting up rig... Please wait.", "Przygotowywanie komputera... Zaczekaj."}, {"Waiting for NVIDIA session ads or confirmation...", "Oczekiwanie na reklamy sesji lub potwierdzenie NVIDIA..."},
            {"Waiting... Status: {0}", "Oczekiwanie... Stan: {0}"}, {"Updating the game", "Aktualizowanie gry"},
            {"Waiting for an available cloud rig", "Oczekiwanie na dostępny komputer w chmurze"}, {"Your cloud rig will start automatically when it is ready.", "Komputer w chmurze uruchomi się automatycznie, gdy będzie gotowy."},
            {"NVIDIA is patching this title. Waiting before retry {0} of 2.", "NVIDIA aktualizuje grę. Oczekiwanie przed próbą {0} z 2."},
            {"Connecting to the streaming server", "Łączenie z serwerem transmisji"}, {"The rig is ready. Configuring the secure network path.", "Komputer jest gotowy. Konfigurowanie bezpiecznego połączenia."},
            {"Starting the video stream", "Uruchamianie obrazu"}, {"Negotiating WebRTC and waiting for the first clean frame.", "Konfigurowanie WebRTC i oczekiwanie na pierwszą poprawną klatkę."},
        }},
        {"zh-CN", {
            {"Advertisement required", "需要观看广告"}, {"Waiting for NVIDIA to provide the Free-tier advertisement details.", "正在等待 NVIDIA 提供免费方案广告信息。"},
            {"Free-tier advertisement ready", "免费方案广告已准备就绪"}, {" ({0} seconds)", "（{0} 秒）"}, {". Prepared format: {0}", "。格式：{0}"},
            {". Playback will begin automatically.", "。即将自动播放。"}, {"Advertisement complete", "广告播放完毕"},
            {"Returning to cloud rig preparation.", "返回云端设备准备流程。"}, {"NVIDIA is updating this game...\nThe session will start automatically when patching finishes.", "NVIDIA 正在更新游戏...\n更新完成后会自动开始会话。"},
            {"In queue... Position: {0}", "正在排队... 位置：{0}"}, {"Setting up rig... Position: {0}", "正在设置云端设备... 位置：{0}"},
            {"Setting up rig... Please wait.", "正在设置云端设备... 请稍候。"}, {"Waiting for NVIDIA session ads or confirmation...", "正在等待 NVIDIA 会话广告或确认..."},
            {"Waiting... Status: {0}", "正在等待... 状态：{0}"}, {"Updating the game", "正在更新游戏"},
            {"Waiting for an available cloud rig", "正在等待可用的云端设备"}, {"Your cloud rig will start automatically when it is ready.", "云端设备准备就绪后将自动启动。"},
            {"NVIDIA is patching this title. Waiting before retry {0} of 2.", "NVIDIA 正在更新游戏。等待后进行第 {0}/2 次尝试。"},
            {"Connecting to the streaming server", "正在连接串流服务器"}, {"The rig is ready. Configuring the secure network path.", "云端设备已就绪。正在配置安全网络连接。"},
            {"Starting the video stream", "正在启动视频串流"}, {"Negotiating WebRTC and waiting for the first clean frame.", "正在配置 WebRTC 并等待第一个完整画面。"},
        }},
    };
    const auto found = dictionaries.find(code);
    return found == dictionaries.end() ? empty : found->second;
}

const Dictionary& UpdateDictionaryFor(const std::string& code)
{
    static const Dictionary empty;
    static const std::unordered_map<std::string, Dictionary> dictionaries = {
        {"ru", {
            {"Updates", "Обновления"}, {"Updates are checked only when you choose this action.", "Проверка обновлений выполняется только вручную."},
            {"Installed version", "Установленная версия"}, {"Check for updates", "Проверить обновления"}, {"Check", "Проверить"},
            {"Check the official NVStreamNX GitHub release and install a newer NRO safely.", "Проверить официальный релиз NVStreamNX на GitHub и безопасно установить новую версию."},
            {"Checking for updates...", "Проверка обновлений..."}, {"Update check failed", "Не удалось проверить обновления"},
            {"NVStreamNX is up to date", "NVStreamNX уже обновлён"}, {"You already have the latest version ({0}).", "У вас уже установлена актуальная версия ({0})."},
            {"Update available", "Доступно обновление"}, {"Version {0} is available. Installed version: {1}.", "Доступна версия {0}. Установлена версия: {1}."},
            {"Download and install it now?", "Скачать и установить её сейчас?"}, {"Install", "Установить"}, {"Downloading update...", "Загрузка обновления..."},
            {"Update installation failed", "Не удалось установить обновление"}, {"Update ready", "Обновление готово"},
            {"Version {0} was downloaded and verified. NVStreamNX will return to HOME while the update is installed.", "Версия {0} загружена и проверена. Во время установки NVStreamNX вернётся в меню HOME."},
            {"Install update", "Установить обновление"}, {"Later", "Позже"}, {"HOME shortcut created", "Ярлык HOME создан"},
            {"The shortcut for {0} was created successfully and added to the HOME Menu.", "Ярлык для {0} успешно создан и добавлен в меню HOME."},
            {"HOME shortcut installation failed", "Не удалось установить ярлык HOME"}
        }},
        {"uk", {
            {"Updates", "Оновлення"}, {"Updates are checked only when you choose this action.", "Перевірка оновлень виконується лише вручну."},
            {"Installed version", "Встановлена версія"}, {"Check for updates", "Перевірити оновлення"}, {"Check", "Перевірити"},
            {"Check the official NVStreamNX GitHub release and install a newer NRO safely.", "Перевірити офіційний реліз NVStreamNX на GitHub і безпечно встановити нову версію."},
            {"Checking for updates...", "Перевірка оновлень..."}, {"Update check failed", "Не вдалося перевірити оновлення"},
            {"NVStreamNX is up to date", "NVStreamNX уже оновлено"}, {"You already have the latest version ({0}).", "У вас уже встановлена актуальна версія ({0})."},
            {"Update available", "Доступне оновлення"}, {"Version {0} is available. Installed version: {1}.", "Доступна версія {0}. Встановлена версія: {1}."},
            {"Download and install it now?", "Завантажити та встановити її зараз?"}, {"Install", "Встановити"}, {"Downloading update...", "Завантаження оновлення..."},
            {"Update installation failed", "Не вдалося встановити оновлення"}, {"Update ready", "Оновлення готове"},
            {"Version {0} was downloaded and verified. NVStreamNX will return to HOME while the update is installed.", "Версію {0} завантажено й перевірено. Під час встановлення NVStreamNX повернеться до меню HOME."},
            {"Install update", "Встановити оновлення"}, {"Later", "Пізніше"}, {"HOME shortcut created", "Ярлик HOME створено"},
            {"The shortcut for {0} was created successfully and added to the HOME Menu.", "Ярлик для {0} успішно створено й додано до меню HOME."},
            {"HOME shortcut installation failed", "Не вдалося встановити ярлик HOME"}
        }},
        {"es", {
            {"Updates", "Actualizaciones"}, {"Updates are checked only when you choose this action.", "Las actualizaciones se comprueban solo de forma manual."},
            {"Installed version", "Versión instalada"}, {"Check for updates", "Buscar actualizaciones"}, {"Check", "Comprobar"},
            {"Check the official NVStreamNX GitHub release and install a newer NRO safely.", "Comprueba la versión oficial de NVStreamNX en GitHub e instala un NRO nuevo de forma segura."},
            {"Checking for updates...", "Buscando actualizaciones..."}, {"Update check failed", "No se pudo buscar actualizaciones"},
            {"NVStreamNX is up to date", "NVStreamNX está actualizado"}, {"You already have the latest version ({0}).", "Ya tienes la versión más reciente ({0})."},
            {"Update available", "Actualización disponible"}, {"Version {0} is available. Installed version: {1}.", "La versión {0} está disponible. Versión instalada: {1}."},
            {"Download and install it now?", "¿Descargarla e instalarla ahora?"}, {"Install", "Instalar"}, {"Downloading update...", "Descargando actualización..."},
            {"Update installation failed", "No se pudo instalar la actualización"}, {"Update ready", "Actualización lista"},
            {"Version {0} was downloaded and verified. NVStreamNX will return to HOME while the update is installed.", "La versión {0} se descargó y verificó. NVStreamNX volverá al menú HOME durante la instalación."},
            {"Install update", "Instalar actualización"}, {"Later", "Más tarde"}, {"HOME shortcut created", "Acceso directo HOME creado"},
            {"The shortcut for {0} was created successfully and added to the HOME Menu.", "El acceso directo de {0} se creó correctamente y se añadió al menú HOME."},
            {"HOME shortcut installation failed", "No se pudo instalar el acceso directo HOME"}
        }},
        {"it", {
            {"Updates", "Aggiornamenti"}, {"Updates are checked only when you choose this action.", "Gli aggiornamenti vengono controllati solo manualmente."},
            {"Installed version", "Versione installata"}, {"Check for updates", "Controlla aggiornamenti"}, {"Check", "Controlla"},
            {"Check the official NVStreamNX GitHub release and install a newer NRO safely.", "Controlla la release ufficiale di NVStreamNX su GitHub e installa in sicurezza un NRO più recente."},
            {"Checking for updates...", "Controllo aggiornamenti..."}, {"Update check failed", "Controllo aggiornamenti non riuscito"},
            {"NVStreamNX is up to date", "NVStreamNX è aggiornato"}, {"You already have the latest version ({0}).", "Hai già la versione più recente ({0})."},
            {"Update available", "Aggiornamento disponibile"}, {"Version {0} is available. Installed version: {1}.", "È disponibile la versione {0}. Versione installata: {1}."},
            {"Download and install it now?", "Scaricarla e installarla ora?"}, {"Install", "Installa"}, {"Downloading update...", "Download aggiornamento..."},
            {"Update installation failed", "Installazione aggiornamento non riuscita"}, {"Update ready", "Aggiornamento pronto"},
            {"Version {0} was downloaded and verified. NVStreamNX will return to HOME while the update is installed.", "La versione {0} è stata scaricata e verificata. NVStreamNX tornerà al menu HOME durante l'installazione."},
            {"Install update", "Installa aggiornamento"}, {"Later", "Più tardi"}, {"HOME shortcut created", "Collegamento HOME creato"},
            {"The shortcut for {0} was created successfully and added to the HOME Menu.", "Il collegamento per {0} è stato creato e aggiunto al menu HOME."},
            {"HOME shortcut installation failed", "Installazione del collegamento HOME non riuscita"}
        }},
        {"fr", {
            {"Updates", "Mises à jour"}, {"Updates are checked only when you choose this action.", "Les mises à jour sont vérifiées uniquement manuellement."},
            {"Installed version", "Version installée"}, {"Check for updates", "Rechercher des mises à jour"}, {"Check", "Vérifier"},
            {"Check the official NVStreamNX GitHub release and install a newer NRO safely.", "Vérifiez la version officielle de NVStreamNX sur GitHub et installez un NRO plus récent en toute sécurité."},
            {"Checking for updates...", "Recherche de mises à jour..."}, {"Update check failed", "Échec de la recherche de mises à jour"},
            {"NVStreamNX is up to date", "NVStreamNX est à jour"}, {"You already have the latest version ({0}).", "Vous avez déjà la dernière version ({0})."},
            {"Update available", "Mise à jour disponible"}, {"Version {0} is available. Installed version: {1}.", "La version {0} est disponible. Version installée : {1}."},
            {"Download and install it now?", "La télécharger et l'installer maintenant ?"}, {"Install", "Installer"}, {"Downloading update...", "Téléchargement de la mise à jour..."},
            {"Update installation failed", "Échec de l'installation de la mise à jour"}, {"Update ready", "Mise à jour prête"},
            {"Version {0} was downloaded and verified. NVStreamNX will return to HOME while the update is installed.", "La version {0} a été téléchargée et vérifiée. NVStreamNX reviendra au menu HOME pendant l'installation."},
            {"Install update", "Installer la mise à jour"}, {"Later", "Plus tard"}, {"HOME shortcut created", "Raccourci HOME créé"},
            {"The shortcut for {0} was created successfully and added to the HOME Menu.", "Le raccourci de {0} a été créé et ajouté au menu HOME."},
            {"HOME shortcut installation failed", "Échec de l'installation du raccourci HOME"}
        }},
        {"pl", {
            {"Updates", "Aktualizacje"}, {"Updates are checked only when you choose this action.", "Aktualizacje są sprawdzane tylko ręcznie."},
            {"Installed version", "Zainstalowana wersja"}, {"Check for updates", "Sprawdź aktualizacje"}, {"Check", "Sprawdź"},
            {"Check the official NVStreamNX GitHub release and install a newer NRO safely.", "Sprawdź oficjalne wydanie NVStreamNX na GitHub i bezpiecznie zainstaluj nowszy plik NRO."},
            {"Checking for updates...", "Sprawdzanie aktualizacji..."}, {"Update check failed", "Nie udało się sprawdzić aktualizacji"},
            {"NVStreamNX is up to date", "NVStreamNX jest aktualny"}, {"You already have the latest version ({0}).", "Masz już najnowszą wersję ({0})."},
            {"Update available", "Dostępna aktualizacja"}, {"Version {0} is available. Installed version: {1}.", "Dostępna jest wersja {0}. Zainstalowana wersja: {1}."},
            {"Download and install it now?", "Pobrać i zainstalować ją teraz?"}, {"Install", "Zainstaluj"}, {"Downloading update...", "Pobieranie aktualizacji..."},
            {"Update installation failed", "Nie udało się zainstalować aktualizacji"}, {"Update ready", "Aktualizacja gotowa"},
            {"Version {0} was downloaded and verified. NVStreamNX will return to HOME while the update is installed.", "Wersja {0} została pobrana i zweryfikowana. Podczas instalacji NVStreamNX wróci do menu HOME."},
            {"Install update", "Zainstaluj aktualizację"}, {"Later", "Później"}, {"HOME shortcut created", "Utworzono skrót HOME"},
            {"The shortcut for {0} was created successfully and added to the HOME Menu.", "Skrót dla {0} został utworzony i dodany do menu HOME."},
            {"HOME shortcut installation failed", "Nie udało się zainstalować skrótu HOME"}
        }},
        {"zh-CN", {
            {"Updates", "更新"}, {"Updates are checked only when you choose this action.", "仅在手动选择此操作时检查更新。"},
            {"Installed version", "已安装版本"}, {"Check for updates", "检查更新"}, {"Check", "检查"},
            {"Check the official NVStreamNX GitHub release and install a newer NRO safely.", "检查 GitHub 上的 NVStreamNX 官方版本并安全安装较新的 NRO。"},
            {"Checking for updates...", "正在检查更新..."}, {"Update check failed", "检查更新失败"},
            {"NVStreamNX is up to date", "NVStreamNX 已是最新版本"}, {"You already have the latest version ({0}).", "你已安装最新版本（{0}）。"},
            {"Update available", "有可用更新"}, {"Version {0} is available. Installed version: {1}.", "版本 {0} 可用。已安装版本：{1}。"},
            {"Download and install it now?", "现在下载并安装吗？"}, {"Install", "安装"}, {"Downloading update...", "正在下载更新..."},
            {"Update installation failed", "更新安装失败"}, {"Update ready", "更新已准备好"},
            {"Version {0} was downloaded and verified. NVStreamNX will return to HOME while the update is installed.", "版本 {0} 已下载并验证。安装更新时 NVStreamNX 将返回 HOME 菜单。"},
            {"Install update", "安装更新"}, {"Later", "稍后"}, {"HOME shortcut created", "HOME 快捷方式已创建"},
            {"The shortcut for {0} was created successfully and added to the HOME Menu.", "{0} 的快捷方式已成功创建并添加到 HOME 菜单。"},
            {"HOME shortcut installation failed", "HOME 快捷方式安装失败"}
        }}
    };
    const auto found = dictionaries.find(code);
    return found == dictionaries.end() ? empty : found->second;
}

} // namespace

const std::vector<InterfaceLanguageOption>& InterfaceLanguageOptions()
{
    static const std::vector<InterfaceLanguageOption> options = {
        {"en", "English"}, {"zh-CN", "简体中文"}, {"es", "Español"},
        {"ru", "Русский"}, {"it", "Italiano"}, {"fr", "Français"},
        {"pl", "Polski"}, {"uk", "Українська"},
    };
    return options;
}

bool IsSupportedInterfaceLanguage(const std::string& code)
{
    const auto& options = InterfaceLanguageOptions();
    return std::any_of(options.begin(), options.end(), [&code](const auto& option) {
        return option.code == code;
    });
}

std::string InterfaceLanguageLabel(const std::string& code)
{
    const auto& options = InterfaceLanguageOptions();
    const auto found = std::find_if(options.begin(), options.end(), [&code](const auto& option) {
        return option.code == code;
    });
    return found == options.end() ? std::string("English") : found->label;
}

void SetInterfaceLanguage(const std::string& code)
{
    g_language = IsSupportedInterfaceLanguage(code) ? code : "en";
}

const std::string& GetInterfaceLanguage()
{
    return g_language;
}

std::string Tr(const std::string& english)
{
    if (g_language == "en" || english.empty())
        return english;
    const auto& dictionary = DictionaryFor(g_language);
    const auto found = dictionary.find(english);
    if (found != dictionary.end())
        return found->second;
    const auto& completion_dictionary = CompletionDictionaryFor(g_language);
    const auto completion_found = completion_dictionary.find(english);
    if (completion_found != completion_dictionary.end())
        return completion_found->second;
    const auto& search_launch_dictionary = SearchAndLaunchDictionaryFor(g_language);
    const auto search_launch_found = search_launch_dictionary.find(english);
    if (search_launch_found != search_launch_dictionary.end())
        return search_launch_found->second;
    const auto& session_launch_dictionary = SessionLaunchDictionaryFor(g_language);
    const auto session_launch_found = session_launch_dictionary.find(english);
    if (session_launch_found != session_launch_dictionary.end())
        return session_launch_found->second;
    const auto& server_dictionary = ServerDictionaryFor(g_language);
    const auto server_found = server_dictionary.find(english);
    if (server_found != server_dictionary.end())
        return server_found->second;
    const auto& update_dictionary = UpdateDictionaryFor(g_language);
    const auto update_found = update_dictionary.find(english);
    return update_found == update_dictionary.end() ? english : update_found->second;
}

std::string Tr(const char* english)
{
    return Tr(english ? std::string(english) : std::string {});
}

std::string TrFormat(const std::string& english, const std::vector<std::string>& values)
{
    std::string result = Tr(english);
    for (size_t index = 0; index < values.size(); ++index)
    {
        const std::string marker = "{" + std::to_string(index) + "}";
        size_t position = 0;
        while ((position = result.find(marker, position)) != std::string::npos)
        {
            result.replace(position, marker.size(), values[index]);
            position += values[index].size();
        }
    }
    return result;
}

} // namespace opennow
