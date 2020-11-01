clear all; close all; clc;

%% load data
%Find all images in subfolders with .jpg extension.
image_files = dir ('**/*.jpg');
%Expression for only folders of interest.
expression_female = 'female';
expression_male = 'male';
expression_female_test = 'female_test';
expression_male_test = 'male_test';

% 180x200 RGB Images
females = []; % (108000x299 (180*200*3 x number of photos))
males = [];
females_test = []; 
males_test = [];
for i=1:length(image_files)
    % check for correct directory.
    if(~isempty(regexp(image_files(i).folder,expression_female)))
        path = strcat(image_files(i).folder,'\',image_files(i).name);
        % figure; imshow(imread(path));
        female = double(imread(path));
        female = reshape(female,[numel(female),1]);
        females = [females female];
    elseif(~isempty(regexp(image_files(i).folder,expression_male)))
        path = strcat(image_files(i).folder,'\',image_files(i).name);
        % figure; imshow(imread(path));
        male = double(imread(path));
        male = reshape(male,[numel(male),1]);
        males = [males male];
    elseif(~isempty(regexp(image_files(i).folder,expression_female_test)))
        path = strcat(image_files(i).folder,'\',image_files(i).name);
        % figure; imshow(imread(path));
        female_test = double(imread(path));
        female_test = reshape(female_test,[numel(female_test),1]);
        females_test = [females_test female_test];
    elseif(~isempty(regexp(image_files(i).folder,expression_male_test)))
        path = strcat(image_files(i).folder,'\',image_files(i).name);
        % figure; imshow(imread(path));
        male_test = double(imread(path));
        male_test = reshape(male_test,[numel(male_test),1]);
        males_test = [males_test male_test];
    end
end

%% train-test preparation
training_data = [males females];
% Men = 1, Women = 2 - add more classes if needed
% test_data = [males_test(:,1:5) females_test(:,1:5)];  % initial testing on less data
% test_GT = [ones(1,5) 2*ones(1,5)];
test_data = [males_test females_test];
test_GT = [ones(1,size(males_test,2)) 2*ones(1,size(females_test,2))];  % Ground truth for testing data

% randomly shuffle test data
for i = 1:size(test_data,2)
    toBeSwapped = randi(size(test_data,2), 1, 2);
    test_data(:,[toBeSwapped(1) toBeSwapped(2)]) = test_data(:,[toBeSwapped(2) toBeSwapped(1)]);
    test_GT(:,[toBeSwapped(1) toBeSwapped(2)]) = test_GT(:,[toBeSwapped(2) toBeSwapped(1)]);
end

%% solving p0 problem using Orthogonal Matching Pursuit
x_list = [];

for i = 1:size(test_data,2)
    i
    test_image = test_data(:,i);
    e0 = 0.0001;
    N = size(training_data,2);
    
    iter_max = 20;  % tricky parameter (shouldn't more be better?)

    k = 0;
    iter = 0;
    x_est = zeros(N,1);
    r = test_image - training_data*x_est;
    S = [];
    
    % stopping condition
    while norm(r)>e0 && iter<iter_max && k<N
        [maxInnerProduct, maxIP_index] = max(abs(training_data'*r));
    
        % ignore if already maxIP_index already part of Support
        if sum(S==maxIP_index)>0 && k~=0
            iter = iter + 1;
            continue
        end
        % update support
        S = [S maxIP_index];
        S = sort(S);
        training_data_s = training_data(:,S);
    
        % update x_est
        x_s = pinv(training_data_s)*test_image;
        x_est(S) = x_s;
    
        % update residual
        r = test_image - training_data*x_est;
    
        k = k + 1;
        iter = iter + 1;
    end
    
    x_list = [x_list x_est];
end

%% predict
likelihood = zeros(2,size(test_data,2));  % probability-like quantity
% first 300 cols of training data males, next "299" cols "females"
% loop convenient if multiple classes
likelihood(1,:) = sum(x_list(1:900,:));
likelihood(2,:) = sum(x_list(900:1199,:));
[largerScore pred] = max(likelihood,[],1);
Accuracy = sum(pred==test_GT)/numel(test_GT)

% interestingly, males harder to predict (male-female 'likelihood' close)
% What if we base decision on this, i.e., on difference of likehoods
likelihood_diff = abs(likelihood(1,:) - likelihood(2,:));
pred_via_diff = ones(1,length(likelihood_diff));
diff_thresh = 0.7;
for i = 1:length(likelihood_diff)
    if(likelihood_diff(i)>diff_thresh)
        pred_via_diff(i) = 2;
    end
end
AccuracyThroughDiff = sum(pred_via_diff==test_GT)/numel(test_GT)