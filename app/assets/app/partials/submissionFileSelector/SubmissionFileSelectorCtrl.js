angular.module('codepuppy').controller('SubmissionFileSelectorCtrl',
['$scope', '$routeParams', '$fileUploader', '$http', '$modal', function($scope, $routeParams, $fileUploader, $http, $modal) {
  
  $scope.fileClicked = function(file) {
    $scope.$parent.$parent.selectedFile = file;
  };

}]);
