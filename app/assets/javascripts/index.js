codepuppy = angular.module('codepuppy', ['ngRoute']);

codepuppy.config([
  '$routeProvider', function($routeProvider) {
    $routeProvider.when('/', {
      templateUrl: 'assets/home.html',
      controller: 'HomeCtrl'
    }).otherwise({
      templateUrl: 'assets/errors/404.html'
    });
  }
]);

codepuppy.controller('HomeCtrl', function($scope) {
  $scope.name = 'World';
});
