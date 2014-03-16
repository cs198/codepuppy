angular.module('codepuppy').controller('LoginPaneCtrl', ['$scope', 'session',
        function($scope, session)
{
    $scope.logout = function() {
        session.logout();
        alert('You just logged out.');
        window.location = 'http://cs198.stanford.edu';
    };
}]);
