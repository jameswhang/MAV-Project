% analytic based TME

% key1; match1; key2; match2; consensus; tm

readMatch();
% consensus = [1 2 3 4 5];
trans = [1 1 1];

KeyOldOrig = KeyOld;
KeyNewOrig = KeyNew;

iter =1;

% while 1
% iter = iter +1 ;
% rng('shuffle');
% perm = randperm(length(KeyOldOrig));
% 
% KeyOld = KeyOldOrig(consensus,:);
% KeyNew = KeyNewOrig(consensus,:);
% % if minimize the entire matched set

% calculate statistics

meanX = mean(KeyNew); % 
meanY = mean(KeyOld);

covxy = zeros(3);
for ii = 1:length(KeyNew)
varx(ii,:) = (norm(KeyNew(ii,:)-meanX))^2;
% vary(ii,:) =  norm(KeyOld(ii,:)-meanY);
covxy = covxy + (KeyOld(ii,:)-meanY)'*(KeyNew(ii,:)-meanX);
end

varx = mean(varx);
covxy = covxy/length(KeyNew);

singularValues = svd(covxy);

[U S V] = svd(covxy);
s = eye(3);  % deal with reflection issue
tm = eye(4); 

% transformation estimate 
scale = 1/varx;
detCovXY = det(covxy);

if (abs(detCovXY)<1e-12)  %very small
    detUV = det(U)*det(V);
    if(detUV < 0)
        s(3,3) = -1;
        scale = scale*(sum(singularValues(1:2))-singularValues(3));
%         scale = scale * 
    else
        scale = sum(singularValues)*scale; 
    end
    
else if (detCovXY<0)
       s(3,3) = -1;
       scale = scale*(sum(singularValues(1:2))-singularValues(3));
    else 
       scale = sum(singularValues)*scale; 
       disp('normal case');
    end
end

rot = U * s * V';

rotm2eul(rot)

trans = meanY' - scale*rot*meanX';

pause

% end

% disp('finished result');
% rpy = rotm2eul(rot)
% trans
% iter
% consensus



% KeyOld vs KeyNew
