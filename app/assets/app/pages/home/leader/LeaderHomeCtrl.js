angular.module('codepuppy').controller('LeaderHomeCtrl', ['$scope', '$http',
'$location', function($scope, $http, $location)
{
    $http({
        method: 'GET',
        url: '/courses/active.json',
        cache: true
    }).success(function(data, status, headers, config) {
        if (data.length > 0) {
            var path = '/courses/' + data[0].id;
            $location.path(path);
        }
    });
}]);
