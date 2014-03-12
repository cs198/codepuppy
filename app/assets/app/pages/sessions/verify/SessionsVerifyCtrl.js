angular.module('codepuppy').controller('SessionsVerifyCtrl',
    ['$scope', '$http', '$routeParams', '$location',
        function($scope, $http, $routeParams, $location)
    {
        $http({
            method: 'POST',
            url: '/sessions/verify',
            params: {
                token: $routeParams.token,
                user_system_id: $routeParams.user_system_id,
                hmac: $routeParams.hmac
            }
        }).success(function(data) {
            // TODO: actually login the user
            alert('login verification success!');
            console.log(data);
            $location.path($routeParams.forward_url);
        }).error(function(data, status, headers, config) {
            // TODO: handle gracefully
            alert('Could not verify login!');
            console.log(data);
            $location.path('/');
        });
    }
]);
