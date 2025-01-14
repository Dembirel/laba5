    filename = 'crc_time.txt';
    data = load(filename);
    
    if size(data, 2) ~= 2
        error('err');
    end
    
    poly_numbers = data(:, 1);
    times = data(:, 2);
    
    hold on;
    plot(poly_numbers, times, '-', 'LineWidth', 1);
    xlabel('Номер ');
    ylabel('микросекунды');
    hold off;