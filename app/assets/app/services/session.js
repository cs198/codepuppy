angular.module('codepuppy').service('session', [
        '$location', '$http', '$cookies',
        function($location, $http, $cookies) {
    this.authenticate = function() {
        // generate token and redirect to webauth
        var loginWebauth = function() {
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
                url: '/sessions/generate.json'
            }).success(function(data) {
                $cookies.api_token = data.token;
                $http.defaults.headers.common.HTTP_AUTHORIZATION =
                    data.token;
                redirectToWebauth(data.token);
            }).error(function(data, status, headers, config) {
                // TODO: handle gracefully
                alert('Could not generate token!');
                console.log(data);
            });
        };

        loginWebauth();
    };

    this.authenticated = function() {
        // check if token exists
        if (!$http.defaults.headers.common.HTTP_AUTHORIZATION) {
            if (!$cookies.api_token) {
                return false;
            }

            $http.defaults.headers.common.HTTP_AUTHORIZATION =
                $cookies.api_token;
        }

        // verify token
        $http({
            method: 'POST',
            url: '/sessions/verify.json'
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
    };

    this.logout = function() {
        // terminate token
        if ($http.defaults.headers.common.HTTP_AUTHORIZATION) {
            delete $http.defaults.headers.common.HTTP_AUTHORIZATION;
        }
    };
}]);
