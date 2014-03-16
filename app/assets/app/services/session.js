angular.module('codepuppy').service('session', [
        '$location', '$http', '$cookies',
        function($location, $http, $cookies) {
    this.authenticate = function() {
        // generate token and redirect to webauth
        var loginWebauth = function(token) {
            var WEBAUTH_LOCATION = 'https://www.stanford.edu/~rawatson' +
                '/cgi-bin/codepuppy/auth.py';
            var baseUrl = $location.protocol() + '://' + $location.host() +
                ':' + $location.port();
            var params = {
                forward_url: $location.path(),
                verify_url: baseUrl + '/sessions/verify',
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
            $http.defaults.headers.common.AUTHORIZATION =
                data.token;
            loginWebauth(data.token);
        }).error(function(data, status, headers, config) {
            // TODO: handle gracefully
            alert('Could not generate token!');
        });
    };

    this.checkAuthenticated = function(successCallback, failureCallback) {
        // check if token exists
        if (!$cookies.api_token) {
            failureCallback();
            return;
        }

        $http.defaults.headers.common.AUTHORIZATION = $cookies.api_token;

        // verify token
        $http({
            method: 'POST',
            url: '/sessions/verify.json'
        }).success(function(data) {
            successCallback();
        }).error(function(data, status, headers, config) {
            // TODO: handle gracefully
            failureCallback();
        });
    };

    this.logout = function() {
        // terminate token
        if ($http.defaults.headers.common.AUTHORIZATION) {
            delete $http.defaults.headers.common.AUTHORIZATION;
        }
        if ($cookies.api_token) {
            delete $cookies.api_token;
        }
    };
}]);
