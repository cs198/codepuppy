angular.module('codepuppy').controller('StudentAssignmentCtrl', ['$scope',
    '$routeParams', '$fileUploader', '$http', '$sce',
    function($scope, $routeParams, $fileUploader, $http, $sce)
{
  $scope.message = 'I\'m an assignment page for the course ' +
      $routeParams.courseID;
  $scope.message += ' and assignment ' + $routeParams.assignmentID;
  $scope.uploadPath = '/#/courses/' + $routeParams.courseID + '/assignments/' +
      $routeParams.assignmentID + '/submit';
}]);
