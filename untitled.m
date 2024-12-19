% Чтение данных из файлов
probability_7bits = [];
errors_7bits = [];
probability_3bits = [];
errors_3bits = [];

% Чтение данных из файла probability_7bits.txt
fileID = fopen('probability_7bits.txt', 'r');
while ~feof(fileID)
    line = fgetl(fileID);
    values = strsplit(line, ',');
    probability_7bits(end + 1) = str2double(values{1});
    errors_7bits(end + 1) = str2double(values{2});
end
fclose(fileID);

% Чтение данных из файла probability_3bits.txt
fileID = fopen('probability_3bits.txt', 'r');
while ~feof(fileID)
    line = fgetl(fileID);
    values = strsplit(line, ',');
    probability_3bits(end + 1) = str2double(values{1});
    errors_3bits(end + 1) = str2double(values{2});
end
fclose(fileID);

% Построение графиков
x = 1:25;

% График вероятностей
figure('Position', [100, 100, 1300, 1000]);
plot(x, probability_7bits, 'DisplayName', '7 bits');
hold on;
plot(x, probability_3bits, 'DisplayName', '3 bits');
legend show;
grid on;
xlabel('Размер массива * 100');
xlim([1 25]);
title('Вероятности');

% График ошибок
figure('Position', [100, 100, 1300, 1000]);
plot(x, errors_7bits, 'DisplayName', '7 bits');
hold on;
plot(x, errors_3bits, 'DisplayName', '3 bits');
legend show;
grid on;
xlabel('Размер массива * 100');
ylabel('Пропущенных ошибок');
xlim([1 25]);
title('Ошибки');
