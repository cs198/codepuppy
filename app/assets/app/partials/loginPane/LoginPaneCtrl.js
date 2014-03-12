angular.module('codepuppy').controller('LoginPaneCtrl',
    ['$scope', '$location', '$http', function($scope, $location, $http)
    {
        // TODO: rather than use $scope var, use service that manages session
        $scope.loggedIn = false;

        $scope.loginWebauth = function() {
            var WEBAUTH_LOCATION =
                'https://www.stanford.edu/~rawatson/cgi-bin/codepuppy/auth.py';

            var redirectToWebauth = function(token)
            {
                var baseUrl = $location.protocol() + '://' + $location.host() +
                    ':' + $location.port();
                var params = {
                    forward_url: $location.path(),
                    verify_url: baseUrl + '/#/sessions/verify',
                    token: token
                };

                var request_url = WEBAUTH_LOCATION + '?' + $.param(params);
                window.location = request_url;
            };

            $http({
                method: 'POST',
                url: '/sessions/generate'
            }).success(function(data) {
                redirectToWebauth(data.token);
            }).error(function(data, status, headers, config) {
                // TODO: handle gracefully
                alert('Could not generate token!');
                console.log(data);
            });
        };

        $scope.logout = function() {
            alert('logout not implemented');
        };
    }
    ]);
