angular.module('codepuppy').controller('AssignmentListCtrl', ['$scope', function($scope) {
  $scope.sortByField = 'date_due';
  $scope.reverse = true;
}]);