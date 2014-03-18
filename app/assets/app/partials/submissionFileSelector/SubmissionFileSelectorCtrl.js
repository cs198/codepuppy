angular.module('codepuppy').controller('SubmissionFileSelectorCtrl',
['$scope', '$routeParams', '$fileUploader', '$http', '$modal', function($scope, $routeParams, $fileUploader, $http, $modal) {
  
  $scope.urlBuilder = function(file) {
    return '/#/courses/' + $routeParams.courseID + '/assignments/' + $routeParams.assignmentID + '/submissions/' + $routeParams.submissionID + '/files/' + file.id; 
  };

}]);
