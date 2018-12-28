function img = screw_image(img)
    % some variables estimated from the 59001:60000 digits we were given
    no_change = 0.45;
    grain_change = 0.1;
    norm_mu = 0;
    norm_sigm = 80;
    for i = 1:28
        for j = 1:28
            pick = rand(1);
            if pick >= 1 - no_change
                continue
            elseif pick >= 1 - no_change - grain_change
                %white/black grain
                if rand(1) > 0.5
                    img(i,j) = 0;
                else
                    img(i,j) = 250;
                end
            else
                %pick from normal distribution
                s = abs(normrnd(norm_mu,norm_sigm));
                img(i,j) = min(img(i,j,1)+s, 255);
            end
        end
    end
end