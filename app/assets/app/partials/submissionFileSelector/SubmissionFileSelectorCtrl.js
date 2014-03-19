angular.module('codepuppy').controller('SubmissionFileSelectorCtrl',
['$scope', '$stateParams', '$fileUploader', '$http', '$modal',
function($scope, $stateParams, $fileUploader, $http, $modal)
{
  $scope.urlBuilder = function(file) {
    return '/#/courses/' + $stateParams.courseID + '/assignments/' +
        $stateParams.assignmentID + '/submissions/' +
        $stateParams.submissionID + '/files/' + file.id;
  };
}]);
