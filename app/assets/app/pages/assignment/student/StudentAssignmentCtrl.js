angular.module('codepuppy').controller('StudentAssignmentCtrl', ['$scope',
    '$routeParams', '$fileUploader', '$http', '$sce',
    function($scope, $routeParams, $fileUploader, $http, $sce)
{
  $http({
    method: 'GET',
    url: '/assignments/' +
    $routeParams.assignmentID + '.json'
  })
  .success(function(data, status, headers, config) {
    if (data.pdf_url) {
      $scope.pdfHTML = $sce.trustAsHtml("<embed width=100% type='application/pdf' src='" + data.pdf_url + "' width=1024 height = 768>");
    } else {
      $scope.pdfURL = null;
    }
  });

  $scope.message = 'I\'m an assignment page for the course ' +
      $routeParams.courseID;
  $scope.message += ' and assignment ' + $routeParams.assignmentID;
  $scope.uploadPath = '/#/courses/' + $routeParams.courseID + '/assignments/' +
      $routeParams.assignmentID + '/submit';
}]);
