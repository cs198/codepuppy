angular.module('codepuppy').controller('NavigationSelectorCtrl',
  function($scope) {
    $scope.expanded = true;
    $scope.lowestLayer = ($scope.selected === null ||
      $scope.selected === undefined);
    if (!$scope.lowestLayer) {
      $scope.expanded = false;
    }

    $scope.onClicked = function() {
      if (!$scope.lowestLayer) {
        $scope.expanded = !$scope.expanded;
      }
    };
  }
);
